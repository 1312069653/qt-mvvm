// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mousemodel.h"
#include <QColor>
#include <QUndoStack>
#include <cmath>
#include <mvvm/core/modeldocuments.h>
#include <mvvm/utils/reallimits.h>
#include <mvvm/model/itemcatalogue.h>
#include <mvvm/utils/numericutils.h>

namespace
{
static const int MouseCount = 7;

std::unique_ptr<ModelView::ItemCatalogue> CreateItemCatalogue()
{
    auto result = std::make_unique<ModelView::ItemCatalogue>();
    result->registerItem<MouseItem>();
    return result;
}

QColor random_color()
{
    auto rndm = []() -> int { return ModelView::Utils::RandInt(0, 255); };
    return QColor(rndm(), rndm(), rndm());
}

} // namespace

MouseItem::MouseItem() : ModelView::CompoundItem("MouseItem")
{
    addProperty(P_COLOR, QColor(Qt::red))->setDisplayName("Color");
    addProperty(P_XPOS, 0.0)->setLimits(ModelView::RealLimits::limitless())->setDisplayName("X");
    addProperty(P_YPOS, 0.0)->setLimits(ModelView::RealLimits::limitless())->setDisplayName("Y");
    addProperty(P_ANGLE, 0.0)->setLimits(ModelView::RealLimits::limitless())->setDisplayName("Angle of yaw");
    addProperty(P_SPEED, 0.0)->setLimits(ModelView::RealLimits::limitless())->setDisplayName("Speed");
}

// ----------------------------------------------------------------------------

MouseModel::MouseModel() : ModelView::SessionModel("MouseModel")
{
    setItemCatalogue(CreateItemCatalogue());
    populate_model();

    setUndoRedoEnabled(true);
    const int max_commands_to_keep = 1000;
    undoStack()->setUndoLimit(max_commands_to_keep);
}

void MouseModel::readFromFile(const QString& name)
{
    auto document = ModelView::CreateJsonDocument({this});
    document->load(name.toStdString());
}

void MouseModel::writeToFile(const QString& name)
{
    auto document = ModelView::CreateJsonDocument({this});
    document->save(name.toStdString());
}

void MouseModel::setUndoPosition(int value)
{
    value = value < 0 ? 0 : value;
    value = value > 100 ? 100 : value;
    int desired_command_id = undoStack()->count() * value / 100;

    if (undoStack()->index() < desired_command_id) {
        while (undoStack()->index() != desired_command_id)
            undoStack()->redo(); // going forward
    } else {
        while (undoStack()->index() != desired_command_id)
            undoStack()->undo(); // going back in time
    }
}

void MouseModel::populate_model()
{
    for (int i = 0; i < MouseCount; ++i) {
        auto item = insertItem<MouseItem>();
        item->setProperty(MouseItem::P_XPOS, std::sin((i * 6.28) / MouseCount) * 200);
        item->setProperty(MouseItem::P_YPOS, std::cos((i * 6.28) / MouseCount) * 200);
        item->setProperty(MouseItem::P_COLOR, QVariant::fromValue(random_color()));
    }
}

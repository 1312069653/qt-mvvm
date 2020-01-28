// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <mvvm/model/comboproperty.h>
#include <mvvm/model/customvariants.h>
#include <mvvm/model/externalproperty.h>
#include <mvvm/viewmodel/defaultcelldecoration.h>

using namespace ModelView;

bool DefaultCellDecoration::hasCustomDecoration(const QModelIndex& index) const
{
    return !cellText(index).empty();
}

std::string DefaultCellDecoration::cellText(const QModelIndex& index) const
{
    auto variant = index.data();

    if (Utils::IsComboVariant(variant))
        return variant.value<ComboProperty>().label();

    else if (Utils::IsBoolVariant(variant))
        return variant.value<bool>() ? "True" : "False";

    else if (Utils::IsExtPropertyVariant(variant))
        return variant.value<ExternalProperty>().text();

    return {};
}

void DefaultCellDecoration::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index)
{
    if (!hasCustomDecoration(index))
        return;

    option->text = QString::fromStdString(cellText(index));
}

/*
	Copyright 2006-2020 The QElectroTech Team
	This file is part of QElectroTech.

	QElectroTech is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	QElectroTech is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with QElectroTech.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "changeelementinformationcommand.h"
#include "element.h"
#include "diagram.h"
#include <QObject>

/**
 * @brief ChangeElementInformationCommand::ChangeElementInformationCommand
 * Default constructor
 * @param elmt : element to change information
 * @param old_info : old info of element
 * @param new_info : new info of element
 */
ChangeElementInformationCommand::ChangeElementInformationCommand(Element *elmt, DiagramContext &old_info, DiagramContext &new_info, QUndoCommand *parent) :
	QUndoCommand (parent),
	m_element    (elmt),
	m_old_info   (old_info),
	m_new_info   (new_info)
{
	setText(QObject::tr("Modifier les informations de l'élément : %1").arg(elmt -> name()));
}

bool ChangeElementInformationCommand::mergeWith(const QUndoCommand *other)
{
	if (id() != other->id()) return false;
	ChangeElementInformationCommand const *undo = static_cast<const ChangeElementInformationCommand*>(other);
	if (m_element != undo->m_element) return false;
	m_new_info = undo->m_new_info;
	return true;
}

/**
 * @brief ChangeElementInformationCommand::undo
 */
void ChangeElementInformationCommand::undo() {
	m_element -> setElementInformations(m_old_info);
	updateProjectDB();
}

/**
 * @brief ChangeElementInformationCommand::redo
 */
void ChangeElementInformationCommand::redo() {
	m_element -> setElementInformations(m_new_info);
	updateProjectDB();
}

void ChangeElementInformationCommand::updateProjectDB()
{
	if(m_element->diagram()) {
		m_element->diagram()->project()->dataBase()->updateDB();
	}
}

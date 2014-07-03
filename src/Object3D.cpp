/*
 * Object3D.cpp
 *
 *  Created on: 03/07/2014
 *      Author: Hernan
 */

#include "Object3D.h"

Object3D::Object3D() {
	// TODO Auto-generated constructor stub

}

void Object3D::load(istream &is){
	string lineStr;
	int count = 0;

	while(!is.eof()){
		getline(is, lineStr);
		cout << "Line: " << lineStr << endl;

		string::size_type pos = lineStr.find(ELEMENT_GROUPS, 0);
		if( pos != string::npos ) {
			count = readElementGroups(is);
		}

		pos = lineStr.find(INCIDENCE, 0);
		if( pos != string::npos ) {
			cout << "Read incidence " << count << endl;
			readIncidence(is, count);
		}

		pos = lineStr.find(COORDINATES, 0);
		if( pos != string::npos ) {
			cout << "Read coordinates " << count << endl;
			readCoordinates(is);
		}
	}
}

int Object3D::readElementGroups(istream &is){
	int totalCount = 0;
	string line;
	getline(is, line);
	int cantElementGroups = atoi(line.c_str());
	cout << "Found " << cantElementGroups << " ELEMENT GROUPS. " << endl;

	for(int i = 1; i <= cantElementGroups; i++){
		// Read Element Group
		string elemGroup;
		getline(is, elemGroup);

		// Group ID
		size_t pos = elemGroup.find(DELIMITER);
		int num = atoi(elemGroup.substr(0, pos).c_str());
		elemGroup.erase(0, pos + DELIMITER.length());

		// Count
		pos = elemGroup.find(DELIMITER);
		int count = atoi(elemGroup.substr(0, pos).c_str());
		elemGroup.erase(0, pos + DELIMITER.length());
		totalCount += count;

		// Type
		pos = elemGroup.find(DELIMITER);
		string type = elemGroup.substr(0, pos);
		elemGroup.erase(0, pos + DELIMITER.length());

		cout << "[Group] " << num << ": " << count << " elements of " << type << "." << endl;

		// Add Element Group
		groups.push_back(*(new ElementGroup(count, type)));
	}

	return totalCount;
}

void Object3D::readIncidence(istream &is, int count){
	// Read incidence
	//for_each(v.begin(), v.end(), &foo);
	int index = 1;
	for (vector<ElementGroup>::iterator i = groups.begin(); i != groups.end(); ++i, ++index)
	{
	    ElementGroup& group = *i;
	    int count = group.getCount();

	    for (int elem = 1; elem <= count; elem++){
	    	string incidence;
	    	getline(is, incidence);

	    	int points[3];
	    	for (int i = 0; i < 3; i++){
	    		size_t pos = incidence.find(DELIMITER);
	    		points[i] = atoi(incidence.substr(0, pos).c_str());
	    		incidence.erase(0, pos + DELIMITER.length());

	    	}
	    	cout << "[Incidence] [G" << index << "] " << points[0] << " - " << points[1] << " - " << points[2] << endl;

	    	group.addElement(*(new Incidence(points[0], points[1], points[2])));
	    }
	}


}

void Object3D::readCoordinates(istream &is){
	string line;
	getline(is, line);
	int cantCoordinates = atoi(line.c_str());
	cout << "Found " << cantCoordinates << " ELEMENT GROUPS. " << endl;

	for(int i = 1; i <= cantCoordinates; i++){
		// Read Element Group
		string coordinate;
		getline(is, coordinate);

		// Coordinate ID
		size_t pos = coordinate.find(DELIMITER);
		int num = atoi(coordinate.substr(0, pos).c_str());
		coordinate.erase(0, pos + DELIMITER.length());

		// X
		pos = coordinate.find(DELIMITER);
		GLfloat x = strtof(coordinate.substr(0, pos).c_str(), 0);
		coordinate.erase(0, pos + DELIMITER.length());

		// Y
		pos = coordinate.find(DELIMITER);
		GLfloat y = strtof(coordinate.substr(0, pos).c_str(), 0);
		coordinate.erase(0, pos + DELIMITER.length());

		// Z
		pos = coordinate.find(DELIMITER);
		GLfloat z = strtof(coordinate.substr(0, pos).c_str(), 0);
		coordinate.erase(0, pos + DELIMITER.length());

		cout << "[Coordinate] " << num << ": " << x << " - " << y << " - " << z << endl;

		// Agregar Element Group
		//grupos.add(new ElementGroup(tokensElemGroup[2], groupCant));

		coordinates[num] = {x, y, z};

	}
}

Object3D::~Object3D() {
	// TODO Auto-generated destructor stub
}


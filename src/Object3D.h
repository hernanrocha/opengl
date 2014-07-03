/*
 * Object3D.h
 *
 *  Created on: 03/07/2014
 *      Author: Hernan
 */

#ifndef OBJECT3D_H_
#define OBJECT3D_H_

#define ELEMENT_GROUPS "*ELEMENT GROUPS"
#define INCIDENCE "*INCIDENCE"
#define COORDINATES "*COORDINATES"

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>

#include <GL/gl.h>


#include "ElementGroup.h"
#include "Incidence.h"

using namespace std;

typedef struct {
	GLfloat x, y, z;
} Vertex3D;


class Object3D {


public:
	// Constructor
	Object3D();

	// Modifiers
	void load(istream &);
	int readElementGroups(istream &);
	void readIncidence(istream &, int);
	void readCoordinates(istream &);

	// Getter
	const vector<ElementGroup>& getGroups() const {
		return groups;
	}

	// Destructor
	virtual ~Object3D();

	const map<int, Vertex3D>& getCoordinates() const {
		return coordinates;
	}

private:
	string DELIMITER = " ";
	vector<ElementGroup> groups;

	map<int, Vertex3D> coordinates;
};

inline std::istream& operator >>(std::istream &is, Object3D &o)
{
    o.load(is);
    return(is);
}

#endif /* OBJECT3D_H_ */

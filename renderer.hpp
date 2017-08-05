/*
 By: Justin Meiners
 
 Copyright (c) 2015 Justin Meiners
 Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <stdio.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


template <class Num=double>
class View
{
public:
    View(Num x, Num y, Num z, Num tx, Num ty, Num tz)
    : eyeX(x), eyeY(y), eyeZ(z), targetX(tx), targetY(ty), targetZ(tz), xAngle(0), yAngle(0), showBonds(true)
    {
        
    }
    
    Num eyeX;
    Num eyeY;
    Num eyeZ;
    
    Num targetX;
    Num targetY;
    Num targetZ;
    
    Num xAngle;
    Num yAngle;
    
    int width;
    int height;
    
    bool showBonds;
};


class Renderer
{
    
public:
    static void ColorForType(Symbol type, double* r, double* g, double*b)
    {
        int index = type % 6;
        int row = type / 6;
        
        if (index == 0 || index == 3 || index == 4) {
            *r = 1.0/(row + 1);
        }
        if (index == 1 || index == 3 || index == 5) {
            *g = 1.0/(row + 1);
        }
        if (index == 2 || index == 4 || index == 5) {
            *b = 1.0/(row + 1);
        }
    }
    
    void Reshape(int width, int height)
    {
        glViewport(0, 0, width, height);
    }
    
    void Render(const Molecule& molecule, const Chemistry<>& chemistry, const View<>& view)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        
        glEnable(GL_DEPTH_TEST);
        
        
        /* 3d view */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45, 4.0/3.0, 1, 100);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        gluLookAt(view.eyeX, view.eyeY, view.eyeZ, view.targetX, view.targetY, view.targetZ, 0, 0, 1);
        

        glEnable(GL_COLOR_MATERIAL);
        
        
        glEnable(GL_LIGHTING);
        
        GLfloat lightPosition[] = {-1.0, -1.0, 1.0, 0.0 };
        
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glEnable(GL_LIGHT0);
        
        
        glRotatef(view.xAngle, 0.0f, 0.0f, 1.0f);
        glRotatef(view.yAngle, 1.0f, 0.0f, 0.0f);
        
        glBegin(GL_TRIANGLES);
        
        for (auto it = molecule.atoms.begin(); it != molecule.atoms.end(); it ++)
        {
            const Atom<>& atom = (*it);
            
            double r = 0.0, g = 0.0, b = 0.0;
            
            ColorForType(atom.symbol, &r, &g, &b);
            
            glColor3d(r, g, b);
            
            double radius = 0.3;
            
            for (auto i = 0; i < 2; i ++)
            {
                double z = 1;
                
                if (i == 1)
                {
                    z = -1;
                }
                
                
                glNormal3f(0.577, 0.577,  0.577 * z);
                glVertex3d(atom.x + radius, atom.y, atom.z);
                glNormal3f(0.577, 0.577,  0.577 * z);
                glVertex3d(atom.x, atom.y + radius, atom.z);
                glNormal3f(0.577, 0.577,  0.577 * z);
                glVertex3d(atom.x, atom.y, atom.z + radius * z);
                
                glNormal3f(-0.577, 0.577,  0.577 * z);
                glVertex3d(atom.x - radius, atom.y, atom.z);
                glNormal3f(-0.577, 0.577,  0.577 * z);
                glVertex3d(atom.x, atom.y + radius, atom.z);
                glNormal3f(-0.577, 0.577,  0.577 * z);
                glVertex3d(atom.x, atom.y, atom.z + radius * z);
                
                glNormal3f(0.577, -0.577,  0.577 * z);
                glVertex3d(atom.x + radius, atom.y, atom.z);
                glNormal3f(0.577, -0.577,  0.577 * z);
                glVertex3d(atom.x, atom.y - radius, atom.z);
                glNormal3f(0.577, -0.577,  0.577 * z);
                glVertex3d(atom.x, atom.y, atom.z + radius * z);
                
                glNormal3f(-0.577, -0.577,  0.577 * z);
                glVertex3d(atom.x - radius, atom.y, atom.z);
                glNormal3f(-0.577, -0.577,  0.577 * z);
                glVertex3d(atom.x, atom.y - radius, atom.z);
                glNormal3f(-0.577, -0.577,  0.577 * z);
                glVertex3d(atom.x, atom.y, atom.z + radius * z);
            }
            
        }
        glEnd();
        
        glDisable(GL_LIGHTING);
        
        
        if (view.showBonds)
        {
            glBegin(GL_LINES);
            
            glColor3d(1.0, 1.0, 1.0);
            
            for (auto it = molecule.bonds.begin(); it != molecule.bonds.end(); it ++)
            {
                const Atom<>& atom = molecule.atoms[std::get<0>(*it)];
                const Atom<>& other = molecule.atoms[std::get<1>(*it)];
                
                glVertex3d(atom.x, atom.y, atom.z);
                glVertex3d(other.x, other.y, other.z);
            }
            
            glEnd();
        }
        
    }
    
private:

};


#endif 

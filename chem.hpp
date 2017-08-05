/*
 By: Justin Meiners
 
 Copyright (c) 2015 Justin Meiners
 Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


#ifndef CHEM_HPP
#define CHEM_HPP

#include <algorithm>
#include <tuple>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <stdio.h>

/* TODO 
 Gldrawarrays
 Vector reserve
 Indexed atom lookup
 Templates chemistry
 Surface normals
 Correct rotation
 Color calculation
 
 */



// supports most main group elements

enum Symbol 
{
    INVALID = -1,
    H = 1, He,
    Li, Be, B, C, N, O, F,Ne,
    Na, Mg, Al, Si, P, S, Cl, Ar,
    K, Ca, Sc, Ti, V, Cr, Mn, Fe, Co, Ni, Cu, Zn, Ga, Ge, As, Se, Br, Kr,
    Rb, Sr, Y, Zr, Nb, Mo, Tc, Ru, Rh, Pd, Ag, Cd, In, Sn, Sb, Te, I, Xe,
    Cs, Ba, Hf=72, Ta, W, Re, Os, Ir, Pt, Au, Hg, Tl, Pb, Bi, Po, At, Rn,
    Fr, Ra,
};

template <class Num=double>
class AtomType
{
public:
    AtomType(Symbol symbol, const std::string& name, Num radius, Num bondRadius)
    : symbol(symbol), name(name), radius(radius), bondRadius(bondRadius)
    {}
    
    Symbol symbol;
    std::string name;
    
    // atomic radius 
    Num radius;
    // covalent radius
    Num bondRadius;
};

template <class Num=double>
struct Atom
{
    Atom(Symbol type) : symbol(type) {}
        
    Symbol symbol;
    Num x;
    Num y;
    Num z;
     
    Num DistanceSquared(const Atom<Num>& other) const
    {
        return (x-other.x)*(x-other.x)+(y-other.y)*(y-other.y)+(z-other.z)*(z-other.z);
    }
};


struct Molecule 
{
    std::vector<Atom<>> atoms;
    std::vector<std::tuple<int, int>> bonds;
};


// chemistry world simulator

template <class Num=double>
class Chemistry
{
public:
    Chemistry()
    {
        
        /* http://www.ibiblio.org/xml/slides/sd2002east/handsonxslt/examples/sortedatoms.html */
        /* http://www.periodictable.com/Properties/A/CovalentRadius.an.html */
        
        /* period 1 */
        table.push_back(AtomType<>(Symbol::H, "H", 2.08, .37));
        table.push_back(AtomType<>(Symbol::He, "He", 1.9, .32));
        
        /* period 2*/
        table.push_back(AtomType<>(Symbol::Li, "Li", 1.55, 1.34));
        table.push_back(AtomType<>(Symbol::Be, "Be", 1.12, .90));
        table.push_back(AtomType<>(Symbol::B, "B", 0.98, .82));
        table.push_back(AtomType<>(Symbol::C, "C", 0.91, .77));
        table.push_back(AtomType<>(Symbol::N, "N", 0.92, .75));
        table.push_back(AtomType<>(Symbol::O, "O", 0.65, .73));
        table.push_back(AtomType<>(Symbol::F, "F", 0.57, .71));
        table.push_back(AtomType<>(Symbol::Ne, "Ne", 0.51, .69));
        
        /* period 3 */
        table.push_back(AtomType<>(Symbol::Na, "Na", 1.9, 1.54));
        table.push_back(AtomType<>(Symbol::Mg, "Mg", 1.6, 1.30));
        table.push_back(AtomType<>(Symbol::Al, "Al", 1.4, 1.118));
        table.push_back(AtomType<>(Symbol::Si, "Si", 1.32, 1.11));
        table.push_back(AtomType<>(Symbol::P, "P", 1.28, 1.06));
        table.push_back(AtomType<>(Symbol::S, "S", 1.27, 1.02));
        table.push_back(AtomType<>(Symbol::Cl, "Cl", 0.97, .99));
        table.push_back(AtomType<>(Symbol::Ar, "Ar", 0.88, .97));
        
        /* period 4 */
        table.push_back(AtomType<>(Symbol::K, "K", 0.88, 1.96));
        table.push_back(AtomType<>(Symbol::Ca, "Ca", 0.88, 1.74));
        table.push_back(AtomType<>(Symbol::Sc, "Sc", 0.88, 1.44));
        table.push_back(AtomType<>(Symbol::Ti, "Ti", 0.88, 1.36));
        table.push_back(AtomType<>(Symbol::V, "V", 0.88, 1.25));
        table.push_back(AtomType<>(Symbol::Cr, "Cr", 0.88, 1.27));
        table.push_back(AtomType<>(Symbol::Mn, "Mn", 0.88, 1.39));
        table.push_back(AtomType<>(Symbol::Fe, "Fe", 0.88, 1.25));
        table.push_back(AtomType<>(Symbol::Co, "Co", 0.88, 1.26));
        table.push_back(AtomType<>(Symbol::Ni, "Ni", 0.88, 1.21));
        table.push_back(AtomType<>(Symbol::Cu, "Cu", 0.88, 1.38));
        table.push_back(AtomType<>(Symbol::Zn, "Zn", 0.88, 1.31));
        table.push_back(AtomType<>(Symbol::Ga, "Ga", 0.88, 1.26));
        table.push_back(AtomType<>(Symbol::Ge, "Ge", 0.88, 1.22));
        table.push_back(AtomType<>(Symbol::As, "As", 0.88, 1.19));
        table.push_back(AtomType<>(Symbol::Se, "Se", 0.88, 1.16));
        table.push_back(AtomType<>(Symbol::Br, "Br", 0.88, 1.14));
        table.push_back(AtomType<>(Symbol::Kr, "Kr", 0.88, 1.10));

        /* period 5 */
        table.push_back(AtomType<>(Symbol::Rb, "Rb", 0.88, 2.11));
        table.push_back(AtomType<>(Symbol::Sr, "Sr", 0.88, 1.92));
        table.push_back(AtomType<>(Symbol::Y, "Y", 0.88, 1.62));
        table.push_back(AtomType<>(Symbol::Zr, "Zr", 0.88, 1.48));
        table.push_back(AtomType<>(Symbol::Nb, "Nb", 0.88, 1.37));
        table.push_back(AtomType<>(Symbol::Mo, "Mo", 0.88, 1.45));
        table.push_back(AtomType<>(Symbol::Tc, "Tc", 0.88, 1.56));
        table.push_back(AtomType<>(Symbol::Ru, "Ru", 0.88, 1.26));
        table.push_back(AtomType<>(Symbol::Rh, "Rh", 0.88, 1.35));
        table.push_back(AtomType<>(Symbol::Pd, "Pd", 0.88, 1.31));
        table.push_back(AtomType<>(Symbol::Ag, "Ag", 0.88, 1.53));
        table.push_back(AtomType<>(Symbol::Cd, "Cd", 0.88, 1.48));
        table.push_back(AtomType<>(Symbol::In, "In", 0.88, 1.44));
        table.push_back(AtomType<>(Symbol::Sn, "Sn", 0.88, 1.41));
        table.push_back(AtomType<>(Symbol::Sb, "Sb", 0.88, 1.38));
        table.push_back(AtomType<>(Symbol::Te, "Te", 0.88, 1.35));
        table.push_back(AtomType<>(Symbol::I, "I", 0.88, 1.33));
        table.push_back(AtomType<>(Symbol::Xe, "Xe", 0.88, 1.30));
        
        /* period 6 */
        table.push_back(AtomType<>(Symbol::Cs, "Cs", 0.88, 2.25));
        table.push_back(AtomType<>(Symbol::Ba, "Ba", 0.88, 1.98));
        table.push_back(AtomType<>(Symbol::Hf, "Hf", 0.88, 1.69));
        table.push_back(AtomType<>(Symbol::Ta, "Ta", 0.88, 1.74));
        table.push_back(AtomType<>(Symbol::W, "Re", 0.88, 1.0));
        table.push_back(AtomType<>(Symbol::Os, "Os", 0.88, 1.0));
        table.push_back(AtomType<>(Symbol::Ir, "Ir", 0.88, 1.0));
        table.push_back(AtomType<>(Symbol::Pt, "Ru", 0.88, 1.0));
        table.push_back(AtomType<>(Symbol::Au, "Au", 0.88, 1.0));
        table.push_back(AtomType<>(Symbol::Hg, "Hg", 0.88, 1.0));
        table.push_back(AtomType<>(Symbol::Ti, "Ti", 0.88, 1.0));
        table.push_back(AtomType<>(Symbol::Pb, "Pb", 0.88, 1.0));
        table.push_back(AtomType<>(Symbol::Bi, "Bi", 0.88, 1.74));
        table.push_back(AtomType<>(Symbol::Po, "Po", 0.88, 1.74));
        table.push_back(AtomType<>(Symbol::At, "At", 0.88, 1.74));
        table.push_back(AtomType<>(Symbol::Rn, "Rn", 0.88, 1.74));
        
        /* period 7 */
        table.push_back(AtomType<>(Symbol::Fr, "Fr", 0.88, 1.74));
        table.push_back(AtomType<>(Symbol::Ra, "Ra", 0.88, 1.74));
    }
    
    Symbol SymbolFromName(const std::string& name) const
    {
   }
    
    bool AtomsCanBond(const Atom<>& atom1, const Atom<>& atom2) const
    {
        double distance = atom1.DistanceSquared(atom2);


        auto it1 = std::find_if(table.cbegin(), table.cend(), [&atom1](const AtomType<>& x) {
            return x.symbol == atom1.symbol;
        }); 

        auto it2 = std::find_if(table.cbegin(), table.cend(), [&atom2](const AtomType<>& x) {
            return x.symbol == atom2.symbol;
        }); 


        if (it1 == table.cend() || it2 == table.cend())
            return false;

       
        double radius1 = (*it1).bondRadius;
        double radius2 = (*it2).bondRadius;
        
        /* 1.1 is multiplied to help fill some gaps */
        double radius =  (radius1+radius2)*(radius1+radius2) * 1.1;
        
        if (distance <= radius) {
            
            return true;
        }
        
        return false;
    }
    
    
    std::vector<AtomType<Num>> table;
};



/* http://wiki.jmol.org/index.php/File_formats/Coordinates#XYZ */

template <class Num=double>
class MoleculeParser {
public:
    MoleculeParser(const Chemistry<Num>& table)
    : chem(table) {}
    
    Molecule ParseMolecule(std::istream& stream)
    {
        Molecule molecule;
        
        std::string line;
        
        char atomName[64];
        
        while (std::getline(stream, line))
        {
            int count;
            Num x, y, z;
            
            if (sscanf(line.c_str(), "%s %lf %lf %lf", atomName, &x, &y, &z) == 4)
            {
                auto it = std::find_if(chem.table.cbegin(), chem.table.cend(), [&atomName](const AtomType<>& x) {
                    return x.name == atomName;
                });
 
                if (it == chem.table.cend())
                    continue; 
                
                Atom<> newAtom((*it).symbol);
                newAtom.x = x;
                newAtom.y = y;
                newAtom.z = z;
                
                molecule.atoms.push_back(newAtom);
            }
            else if (sscanf(line.c_str(), "%i", &count) == 1)
            {
                if (count > 0)
                {
                    molecule.atoms.reserve(count);
                }
            }
        }
        
        
        for (auto it = molecule.atoms.begin(); it != molecule.atoms.end(); it ++)
        {
            const Atom<>& atom = (*it);
            
            for (auto j = molecule.atoms.begin(); j != molecule.atoms.end(); j ++)
            {
                const Atom<>& other = (*j);
                
                if (it != j && chem.AtomsCanBond(atom, other))
                {
                    auto bond = std::make_tuple(it-molecule.atoms.begin(), j-molecule.atoms.begin());
                    molecule.bonds.push_back(bond);
                }
            }
        }
        
        return molecule;
    }
    
    Molecule ParseMolecule(const std::string& file)
    {
        
        std::ifstream stream(file);
        return ParseMolecule(stream);
    }
    
private:
    const Chemistry<Num>& chem;
};


#endif /* chem_hpp */

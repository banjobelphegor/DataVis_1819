//
// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute in modified or unmodified form! 
// Copyright (C) 2015 - 2018 CGV TU Dresden - All Rights Reserved
//
// This file provides some helper functions to save some repetitive vtk code
//

#pragma once

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>


/* For a given render window, this method creates a 3d interactor, opens the window, lets the user interact and 
   then closes the window. */
void doRenderingAndInteraction(vtkSmartPointer<vtkRenderWindow> window);

/* Start an interactor and delete data afterwards. */
void doRenderingAndInteraction(vtkSmartPointer<vtkRenderWindowInteractor> interactor);


/* Generates a vtkRendererWindow from a mapper. Internally, it creates an appropriate actor and attaches it to a renderer.
   The renderer then is attached to a render window, which is returned. 
   The second and third parameter take a reference to an actor and renderer to use the generated classes outside of this method. */
vtkSmartPointer<vtkRenderWindow> createRenderWindowFromMapper(vtkSmartPointer<vtkMapper> mapper,
	vtkSmartPointer<vtkActor>& actor,
	vtkSmartPointer<vtkRenderer>& renderer);

/* Same method as above, except that only the generated actor is assigned to the second parameter. */
vtkSmartPointer<vtkRenderWindow> createRenderWindowFromMapper(vtkSmartPointer<vtkMapper> mapper,
	vtkSmartPointer<vtkActor>& actor);

/* Same method as above, except that only the generated renderer is assigned to the second parameter. */
vtkSmartPointer<vtkRenderWindow> createRenderWindowFromMapper(vtkSmartPointer<vtkMapper> mapper,
	vtkSmartPointer<vtkRenderer>& renderer);

/* Same method as above, except that only the render window will be returned. The actor and renderer are 
   not directly accessible anymore. */
vtkSmartPointer<vtkRenderWindow> createRenderWindowFromMapper(vtkSmartPointer<vtkMapper> mapper);

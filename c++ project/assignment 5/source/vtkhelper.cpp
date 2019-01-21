//
// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute in modified or unmodified form! 
// Copyright (C) 2015 - 2018 CGV TU Dresden - All Rights Reserved
//


#include "vtkhelper.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>



void doRenderingAndInteraction(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
	vtkRenderWindow *window = interactor->GetRenderWindow();

	// set trackball interactor
	interactor->SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());

	// execute render/interaction loop
	interactor->Initialize();
	interactor->Start();

	// close the window afterwards and destroy the resources
	window->Finalize();
}




/* For a given render window, this method creates a 3d interactor, opens the window, lets the user interact and
then closes the window. */
void doRenderingAndInteraction(vtkSmartPointer<vtkRenderWindow> window)
{
	// create interactor (with 'trackball' interactor style)
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkRenderWindowInteractor::New();
	interactor->SetRenderWindow(window);
	interactor->SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());

	// execute render/interaction loop
	interactor->Initialize();
	interactor->Start();

	// close the window afterwards and destroy the resources
	window->Finalize();
}



/* Generates a vtkRendererWindow from a mapper. Internally, it creates an appropriate actor and attaches it to a renderer.
The renderer then is attached to a render window, which is returned.
The second and third parameter take a reference to an actor and renderer to use the generated classes outside of this method. */
vtkSmartPointer<vtkRenderWindow> createRenderWindowFromMapper(vtkSmartPointer<vtkMapper> mapper,
	vtkSmartPointer<vtkActor>& actor,
	vtkSmartPointer<vtkRenderer>& renderer)
{
	// Create an actor with disabled lighting
	actor = vtkActor::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->LightingOff();

	// Create a renderer
	renderer = vtkRenderer::New();
	renderer->AddActor(actor);

	// Create a window, set viewport size and add the renderer
	vtkSmartPointer<vtkRenderWindow> window = vtkRenderWindow::New();
	window->SetSize(640, 480);
	window->AddRenderer(renderer);
	return window;
}



/* Same method as above, except that only the generated actor is assigned to the second parameter. */
vtkSmartPointer<vtkRenderWindow> createRenderWindowFromMapper(vtkSmartPointer<vtkMapper> mapper,
	vtkSmartPointer<vtkActor>& actor)
{
	vtkSmartPointer<vtkRenderer> renderer;
	return createRenderWindowFromMapper(mapper, actor, renderer);
}


/* Same method as above, except that only the generated renderer is assigned to the second parameter. */
vtkSmartPointer<vtkRenderWindow> createRenderWindowFromMapper(vtkSmartPointer<vtkMapper> mapper,
	vtkSmartPointer<vtkRenderer>& renderer)
{
	vtkSmartPointer<vtkActor> actor;
	return createRenderWindowFromMapper(mapper, actor, renderer);
}


/* Same method as above, except that only the render window will be returned. The actor and renderer are
not directly accessible anymore. */
vtkSmartPointer<vtkRenderWindow> createRenderWindowFromMapper(vtkSmartPointer<vtkMapper> mapper)
{
	vtkSmartPointer<vtkActor> actor;
	vtkSmartPointer<vtkRenderer> renderer;
	return createRenderWindowFromMapper(mapper, actor, renderer);
}




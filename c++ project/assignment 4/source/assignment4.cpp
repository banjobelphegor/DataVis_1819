//
// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute in modified or unmodified form! 
// Copyright (C) 2018 CGV TU Dresden - All Rights Reserved
// edited by Yanchen Zhao,Oliver Braun
//

#include <vtkAutoInit.h>
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );
VTK_MODULE_INIT( vtkRenderingFreeType );

// VTK includes
#include <vtkSmartPointer.h>
#include <vtkDEMReader.h>
#include <vtkContourFilter.h>
#include <vtkImageGradientMagnitude.h>
#include <vtkWarpScalar.h>
#include <vtkDataSetMapper.h>

#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRendererCollection.h>

#include <vtkColorTransferFunction.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>
#include <vtkTextRenderer.h>

// standard includes
#include <vector>
#include <algorithm>




// ----- utility functions -----
void setGradientBackground( vtkSmartPointer<vtkRenderer> renderer )
{
	renderer->GradientBackgroundOn();
	renderer->SetBackground( 0.5, 0.5, 0.5 );
	renderer->SetBackground2( 0.9, 0.9, 0.9 );
}
// ----- end of utility functions -----


vtkSmartPointer<vtkRenderWindow> createRenderWindowFromMapper( vtkSmartPointer<vtkMapper> mapper )
{
	//create renderer, window and actor
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> window = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	
	// let's have a fancy background for the canvas
	setGradientBackground( renderer );
	// fallback color, is used whenever mappers do not utilize scalars for coloring
	actor->GetProperty()->SetColor( 1, 1, 1 );

	// connect everything
	actor->SetMapper( mapper );
	renderer->AddActor( actor );
	window->AddRenderer( renderer );
	
	// set window size and return
	window->SetSize( 800, 800 );
	return window;
}


vtkSmartPointer<vtkLookupTable> makeLookupTable() {

	//color transform, linear
	vtkSmartPointer<vtkColorTransferFunction> ctf = vtkSmartPointer<vtkColorTransferFunction>::New();
	ctf->SetColorSpaceToDiverging();
	ctf->AddRGBPoint(0, 0.1, 0.13, 0.35);
	ctf->AddRGBPoint(1, 1, 1, 1);
	//lookuptable
	vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
	lut->SetNumberOfColors(256);
	lut->SetTableRange(680, 2500);    //so that the tick label of legend will be changed
	lut->Build();
	for (int i = 0; i < 256; i++) {
		double color[4] = { 0.0, 0.0, 0.0, 1.0 };
		ctf->GetColor((double)(i) / 256, color);
		lut->SetTableValue(i, color);
	}
	return lut;
}

vtkSmartPointer<vtkRenderWindow> createRenderWindowFromMultipleMappers( std::vector<vtkSmartPointer<vtkMapper>> mappers )
{
	// create renderer and window
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> window = vtkSmartPointer<vtkRenderWindow>::New();

	// let's have a fancy background for the canvas
	setGradientBackground( renderer );

	// connect window and renderer
	window->AddRenderer( renderer );

	// task 4.2
	// for every mapper we create an actor and add it to the renderer )
	
	// instantiate an actor for each mapper, then connect to renderer
	for (unsigned int i = 0; i < mappers.size(); i++) {
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		//fallback color
		actor->GetProperty()->SetColor(1, 1, 1);
		actor->SetMapper(mappers[i]);
		renderer->AddActor(actor);
	}

	// legend
	vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
	vtkSmartPointer<vtkLookupTable> lut = makeLookupTable();
	scalarBar->SetLookupTable(lut);
	scalarBar->SetTitle("Elevation");
	scalarBar->SetNumberOfLabels(6);
	scalarBar->SetMaximumHeightInPixels(400);
	scalarBar->SetMaximumWidthInPixels(80);
	renderer->AddActor2D(scalarBar);

	// set window size and return
	window->SetSize( 800, 800 );
	return window;
}

void doRenderingAndInteraction( vtkSmartPointer<vtkRenderWindow> window )
{
    // create interactor and connect a window
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( window );
	// set an interaction style
	interactor->SetInteractorStyle( vtkInteractorStyleTrackballCamera::New() );

    // execute render/interaction loop
    interactor->Initialize();
    interactor->Start();

    // close the window when finished
    window->Finalize();
}


int main(int argc, char * argv[])
{
    // -- begin of basic visualization network definition --
   
	// 1. create source
	vtkSmartPointer<vtkDEMReader> source = vtkSmartPointer<vtkDEMReader>::New();
	source->SetFileName( "../data/SainteHelens.dem" );

	// filters
	// use the warp filter
	vtkSmartPointer<vtkWarpScalar> warpScalar = vtkSmartPointer<vtkWarpScalar>::New();
	warpScalar->SetInputConnection(source->GetOutputPort());
	warpScalar->SetScaleFactor(2);

	// contour filter
	vtkSmartPointer<vtkContourFilter> contourFilter = vtkSmartPointer<vtkContourFilter>::New();
	// use scaled source as filter input
	contourFilter->SetInputConnection( warpScalar->GetOutputPort() );
	// Draw a contour line every 120 heights
	for (int i = 0; i < 15; i++) {
		contourFilter->SetValue(i, 120 * i + 685 );
	}
	
	// gradient filter
	//vtkSmartPointer<vtkImageGradientMagnitude> gradientFilter = vtkSmartPointer<vtkImageGradientMagnitude>::New();
	// how many dimensions does the data have
	//gradientFilter->SetDimensionality( 2 );
	// use source as filter input
	//gradientFilter->SetInputConnection( source->GetOutputPort() );

	vtkSmartPointer<vtkLookupTable> lut = makeLookupTable();


	// 3.  create mappers
	// a) image mapper, show the warped data in 3d
	vtkSmartPointer<vtkDataSetMapper> imageMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	// connect directly to source (the pipeline is source->imageMapper->...)
	imageMapper->SetInputConnection( warpScalar->GetOutputPort() );
	imageMapper->SetLookupTable(lut);
	// set the range of the scalar values in order to correctly map the colors
	imageMapper->SetScalarRange( 682, 2543 );

	// b) contour mapper, show the regions where the data has a specific value
	vtkSmartPointer<vtkDataSetMapper> contourMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	// connect to the contour filter output (the pipeline is source->contourFilter->contourMapper->...)
	contourMapper->SetInputConnection( contourFilter->GetOutputPort() );
	// avoid z-buffer fighting with small polygon shift
	contourMapper->SetResolveCoincidentTopologyToPolygonOffset();
	// don't use the scalar value to color the line, see fallback coloring via actor in createRenderWindowFromMapper
	contourMapper->ScalarVisibilityOff();
	
	// c) gradient mapper, show the gradient magnitudes as 2D image 
	//vtkSmartPointer<vtkDataSetMapper> gradientMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	// connect to the gradient filter output (the pipeline is source->gradientFilter->gradientMapper->...)
	//gradientMapper->SetInputConnection( gradientFilter->GetOutputPort() );
	// set the range of the gradient magnitudes in order to correctly map the colors
	//gradientMapper->SetScalarRange( 0, 2 );
	
    // -- end of basic visualization network definition --


    // 4. create actors, renderers, render windows 	
	vtkSmartPointer<vtkRenderWindow> imageWindow = createRenderWindowFromMapper( imageMapper );
	vtkSmartPointer<vtkRenderWindow> contourWindow = createRenderWindowFromMapper( contourMapper );
	//vtkSmartPointer<vtkRenderWindow> gradientWindow = createRenderWindowFromMapper( gradientMapper );
	
    // 5. successively show each window and allow user interaction (until it is closed)
	//doRenderingAndInteraction( imageWindow );
	//doRenderingAndInteraction( contourWindow );
	//doRenderingAndInteraction( gradientWindow );

	std::vector<vtkSmartPointer<vtkMapper>> finalMapper;
	finalMapper.push_back(imageMapper);
	finalMapper.push_back(contourMapper);

	vtkSmartPointer<vtkRenderWindow> finalWindow = createRenderWindowFromMultipleMappers(finalMapper);

	doRenderingAndInteraction(finalWindow);
    return 0;
}

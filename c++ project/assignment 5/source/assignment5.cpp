//
// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute in modified or unmodified form! 
// Copyright (C) 2017, 2018 CGV TU Dresden - All Rights Reserved
//edited by Yanchen Zhao, Oliver Braun
//

#include <vtkAutoInit.h>
// Needed for general rendering
VTK_MODULE_INIT(vtkRenderingOpenGL2);
// Needed for interactive controls
VTK_MODULE_INIT(vtkInteractionStyle);
// Guess what that is needed for
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
// Needed for the text on the slider
VTK_MODULE_INIT(vtkRenderingFreeType);

#include "vtkhelper.h"

#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include <vtkXMLImageDataReader.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkSmartVolumeMapper.h>

#include <vtkMarchingCubes.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>

#include <vtkSliderRepresentation2D.h>
#include <vtkSliderWidget.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCommand.h>

class IsoSliderCallback : public vtkCommand {
private:
	vtkSmartPointer<vtkMarchingCubes> isoSurface;
	IsoSliderCallback() {}

public:
	static IsoSliderCallback *New() { return new IsoSliderCallback; }

	void SetData( vtkSmartPointer<vtkMarchingCubes> isoSurface ) { this->isoSurface = isoSurface; }

	virtual void Execute( vtkObject *caller, unsigned long eventId, void *callData ) {
		// Get our slider widget back
		vtkSliderWidget *slider = static_cast<vtkSliderWidget*>(caller);

		// Get the value
		double value = static_cast<vtkSliderRepresentation*>(slider->GetRepresentation())->GetValue();

		// Set new Iso value
		isoSurface->SetValue( 0, value );
		isoSurface->Update();
	}
};

int main()
{
	vtkSmartPointer<vtkXMLImageDataReader> source = vtkSmartPointer<vtkXMLImageDataReader>::New();
	source->SetFileName("../data/ctTorso.vti");

	/*
	// Task 5.2

	// visualize volume directly:
	// * create a vtkSmartVolumeMapper that gets its input from the source
	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();

	// * enable GPU rendering and set the appropriate volume blending
	volumeMapper->SetRequestedRenderModeToGPU();
	volumeMapper->SetInputConnection(source->GetOutputPort());
	volumeMapper->SetBlendModeToComposite();// composite first

	// * create an opacity transfer function as vtkPiecewiseFunction and add density-opacity pairs
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	compositeOpacity->AddPoint(150, 0.0);
	compositeOpacity->AddPoint(255, 1.0);
	
	// * create a color transfer function as vtkColorTransferFunction and add density-color pairs
	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
	color->AddRGBPoint(0, 0, 0, 0);
	color->AddRGBPoint(128, 0.867, 0.867, 0.867);
	color->AddRGBPoint(255, 1, 1, 1);

	// * create a vtkVolumeProperty object, set the opacity and color function and set the 
	//   interpolation type to linear. Turn shading on
	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationTypeToLinear();;
	volumeProperty->SetScalarOpacity(compositeOpacity);
	volumeProperty->SetColor(color);

	// * create the actor as a vtkVolume object and assign the previously created volume mapper and property object
	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	// * create a vtkRenderer and a vtkRenderWindow. (Note that you cannot use the method createRenderWindowFromMapper 	
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> window = vtkSmartPointer<vtkRenderWindow>::New();

	renderer->AddVolume(volume);
	window->SetSize(640, 480);
	window->AddRenderer(renderer);
	//doRenderingAndInteraction(window);
	*/
	// visualize volume via isosurfaces:
	// * generate polygon data from the volume dataset by using a vtkMarchingCubes filter
	vtkSmartPointer<vtkMarchingCubes> polygen = vtkSmartPointer<vtkMarchingCubes>::New();

	// * set number of contours to one, set scalar value of that contour to something meaningful
	polygen->SetInputConnection(source->GetOutputPort());
	polygen->SetValue(0, 150);

	// * manually update the Marching Cubes filter aftwerwards via Update() method to apply the contour value
	polygen->Update();

	// * create vtkDataSetMapper and set input connection, don't use scalars for coloring (set scalar visibility to false)
	vtkSmartPointer<vtkDataSetMapper> dataSetMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	dataSetMapper->SetInputConnection(polygen->GetOutputPort());
	dataSetMapper->SetScalarVisibility(false);

	// * create vtkActor and set mapper as input
	vtkSmartPointer<vtkActor>polygenActor = vtkSmartPointer<vtkActor>::New();
	polygenActor->SetMapper(dataSetMapper);

	// * assign actor to existing renderer
	vtkSmartPointer<vtkRenderer> polygenRenderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> polygenWindow = vtkSmartPointer<vtkRenderWindow>::New();
	polygenRenderer->AddActor(polygenActor);
	polygenWindow->SetSize(640, 480);
	polygenWindow->AddRenderer(polygenRenderer);

	// * create a slider as a slider 2d representation
	vtkSmartPointer<vtkSliderRepresentation2D> slider = vtkSmartPointer<vtkSliderRepresentation2D>::New();
	// * set the minimum and maximum values to correspond to the dataset
	slider->SetMaximumValue(200);
	slider->SetMinimumValue(0);
	slider->SetValue(150);
	// * show a slider title
	slider->SetTitleText("Iso Value");
	// * show the current slider value above the slider with one digit behind the decimal point (setLabelFormat)
	slider->SetLabelFormat("%.1f");
	slider->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
	slider->GetPoint1Coordinate()->SetValue(80, 50);
	slider->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
	slider->GetPoint2Coordinate()->SetValue(560, 50);

	// * create a vtkRenderWindowInteractor and assign a rendering window
	vtkSmartPointer<vtkRenderWindowInteractor> sliderInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	sliderInteractor->SetRenderWindow(polygenWindow);

	// * create a new vtkSliderWidget and assign the previous interactor and representation to it
	vtkSmartPointer<vtkSliderWidget> sliderWidget = vtkSmartPointer<vtkSliderWidget>::New();
	sliderWidget->SetInteractor(sliderInteractor);
	sliderWidget->SetRepresentation(slider);
	sliderWidget->SetAnimationModeToAnimate();
	sliderWidget->EnabledOn();

	// * invoke the callback code:
	// * create an IsoSlider Callback
	vtkSmartPointer<IsoSliderCallback> callback = vtkSmartPointer<IsoSliderCallback>::New();
	// * assign the Marching Cubes data
	callback->SetData(polygen);
	// * assign the callback object to the slider
	sliderWidget->AddObserver(vtkCommand::InteractionEvent, callback);

	// initialize the interactor
	sliderInteractor->Initialize();
	polygenWindow->Render();
	sliderInteractor->Start();

	return 0;
}

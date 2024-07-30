
The world of smart connected devices is dynamic and develops more sophisticated products to enhance our safety, convenience and entertainment. From surveillance to automotive safety systems and from mobile devices to autonomous drones, intelligent image processing and deep learning are fundamental challenges. New generation smart devices are enriched with AI, which is set to elevate embedded imaging solutions as they are essential to the success of many Internet of Things (IoT) applications.

What is Image Quality?

A quality image does not mean a sharp, bright image with high contrast. Image quality (IQ) is a critical issue that encompasses much more than brightness and sharpness. Several factors contribute significantly to the quality of the image a camera delivers. A large amount of math and processing is involved when the camera takes the light from your sensor and transforms it into a good-looking picture. Fortunately, with the use of dedicated Image signal processors (ISPs), digital cameras and smartphones are handling this in an energy-efficient way.

Why do we need camera IQ tuning?

A camera must be able to take quality images in a variety of lighting conditions, including indoors, in strong sunlight, and in darkness. Video footage and images can easily lose colors, critical details and gain image noise in dim light. Many elements make up a whole camera system and work together to get the final image. Image quality defines how well a camera system performs when reproducing an object/scene. Various characteristics of the camera system like sensor, firmware and lens contribute different elements to the overall quality of an image.

We need IQ tuning to achieve the best image/video quality from the camera system, as there are:

Lens distortion, sensor defects, noise, color response
Variations in mechanical, optical systems and electrical characteristics
Measurement criteria including automatic exposure (AE) with brightness and saturation statistics, automatic focus (AF) with contrast statistics, and automatic white balance (AWB) with color statistics
Subjectivity due to individual image quality preferences
Unprocessed images do not accurately depict an actual scene. This is where an ISP’s job begins to obtain the highest image quality possible.

What is Image Signal Processing?

A lens module is paired with an image sensor and an ISP in every camera system.

ISP in a camera system processes the RAW image from the image sensor to a final image.
For attaining the best image quality, the ISP parameters have to be configured iteratively for various lighting conditions and scenarios.
Due to the large number of ISP parameters that must be optimized, the tuning procedure can take up too many weeks to months.

Figure 1: Typical ISP pipeline for digital camera. (Source: PathPartner)

Using “Bayer’s transformation” to get colorful images

The Bayer filter, or Color Filter Array (CFA), acts as a display, allowing light photons of a certain color into each pixel on the sensor.  These sensors capture red, green and blue colors at each photosite.

Only blue photons will travel through the blue filter, similarly, only red and green photons will be allowed to pass through the red and green filter.        

The values from these photosites are then intelligently merged to create full-color pixels using a technique known as “demosaicing,” and then further processed using an ISP pipeline method to obtain high quality.


Figure 2: Converting light information into an ISP processed image. (Source: PathPartner)

As a result, with the help of an image sensor, light received through the optical lens is converted into an electronic form. And this information is directed to an ISP where it is reconstructed as a picture. The ISP does wonders to improve image quality, even when the lighting isn’t optimal for photo taking.           

What are imaging algorithms?

Imaging algorithms are a suite of custom algorithms developed for various use cases to enhance images under varied conditions and in real time; such as:

3A algorithms (AWB, AF, AE)
Low light enhancement
High dynamic range (HDR) algorithms
Software image pipeline
Stereo camera vision algorithms
Array camera software pipeline for bokeh and refocus applications
Depth map generation for ToF camera sensors
Spatial and temporal noise removal
Multi-frame & single-frame based super-resolution
Electronic image stabilization (EIS)
A set of 3A Algorithms (autoexposure, auto balance and autofocus) controls the camera exposure, white balance and focus.


Figure 3: Example of Untuned vs tuned Auto White Balance (AWB) (Source: PathPartner)

Low light enhancement – Enhance image quality in extremely low light conditions:

Automatic color enhancement
Sensor independent implementation
Supports both Bayer and YUV format
2D Noise reduction without detail loss  

Figure 4: Without low light enhancement (L) and with low light enhancement (R) ((Source: PathPartner)

High dynamic range (HDR) images/videos – Resolve issues with dynamic range that are common with low-cost cameras. It improves the pixels and enables the viewer to see a more realistic image/video.

Image registration and de-ghosting algorithms
Ported easily to custom hardware solutions with camera driver access and HAL
Efficient multi -exposure image fusion methods retaining best information from each of the images

Figure 5: Representing a large luminance variation from dark values to bright values. ((Source: PathPartner)

Performing image quality tuning for a surveillance camera system

The camera’s ability to capture an image of a moving vehicle at a very high speed in day/night mode and various frame rates directly impacts on the image quality. To get the best image quality, the image pipeline must be fine-tuned by configuring each ISP block to respond appropriately to dynamic lighting situations, ensuring that cameras generate acceptable images/videos under all lighting conditions.

For this example, key requirements might include:

Capture high-quality images in day-mode with a variety of lighting conditions ranging from full sun to overcast sky
Capture high-quality images in night lighting conditions with the subject illuminated by integrated infrared LED illuminators
A frame rate > (greater) 25 frames / sec at a full sensor resolution of 5 MP
A step-by-step process of tuning each block of ISP is important to avoid unnecessary iterations. This is mainly because if one of the blocks in ISP is not calibrated or tuned correctly, the subsequent blocks suffer and eventually produce bad image quality. By analyzing the hardware capabilities and limitations based on specifications, the tuning process is carried as below:

3A tuning
Objective image quality tuning
Subjective evaluation
Image quality evaluation and competitive benchmarking
IQ tuning needs to be performed for each of the ISP blocks showing in the figures below for day mode and night mode settings:


Figure 6: Day mode settings. (Source: PathPartner)


Figure 7 – Night mode settings. (Source: PathPartner)

Objective measurements

The tuning process involves calculating different parameters of the camera (Ex: dark current, sensor RGB color space, noise model, AWB reference values, distortion model, etc.) to derive initial settings for ISP and 3A modules. These camera parameters are calculated from the images of standard test charts taken at specific and controlled lighting conditions.

Capturing these images usually requires a lab setup with a uniform light source, test charts and measuring equipment such as lux meters and chroma meters. The number of images to capture usually depends on the configuration of dynamic regions created for each of the ISP blocks based on lux levels/exposure time/sensor gains/color temperatures.     

Subjective evaluation


Once the initial tuning is completed, most of the objective quality requirements are typically met, but a subjective image quality evaluation is done to further fine-tune to produce the preferred results. Fine-tuning involves the following steps:

Capturing and analyzing real-life scenes to find any quality issues and gaps in quality expectations.
Modifying the ISP and 3A parameters to fix any quality issues and accommodating preferences or adding calibration data to handle specific scene types.
This process is carried out in several iterations to verify the fixes and make sure no new artifacts are introduced, making sure IQ preference is met.

It is important to measure the image quality during the tuning process to decide the extent of ISP parameter optimization. This IQ testing is carried out in two phases:

Objective IQ Testing

After initial tuning, objective IQ testing is carried out and KPIs were measured, with images of standard test charts like X-rite MCC, ISO, eSFR, etc. taken in different lighting conditions simulated in imaging lab ranging from 2000K to 7500K and 1lux to 10000lux. The test cases and IQ pass/fail criteria are aligned with the intended use case of the camera. 

Subjective IQ Testing:

Various scenarios like indoor, outdoor at various times of the day, landscape, high dynamic range scenes, low lights, mixed lighting conditions, etc. are typically carried out. The captured images might be used for computer vision-based applications to automatically detect a vehicle number plate, type and color.

Various factors are analyzed and fixed for factors like:

Natural colorfulness
Noise level & texture details (aliasing, moiré patterns, sharpening artifacts, text quality in documents, etc.)
Auto white balance (AWB) decision (Different time of the day, dusk and night)
Brightness (AE) and tonal range
Sharpness and resolution of number plates
IQ Benchmark Test

This process involves comparing and benchmarking the image quality of a tuned device under tests with other standard consumer devices. IQ benchmarking for both objective and subjective criteria helps to improve and make the image quality of the device under test significantly comparable to standard consumer products.         

Conclusion

Tuning an image pipeline is a complex task for a particular camera system. Many artifacts degrade the quality of an image. To address these artifacts a camera must embed an image signal processing process needed to generate high-quality digital images. Hence, it is important to understand the purpose of each ISP block and how each stage of the pipeline can be optimized for a particular use case.

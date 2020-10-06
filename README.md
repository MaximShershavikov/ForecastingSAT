![изображение](https://user-images.githubusercontent.com/46265118/94804631-cf03c880-03f3-11eb-9e4c-7a93625cb39b.png)
This program uses the SPG4 calculation methodology to determine the coordinates of the satellite position in the future, the date and UTC time of the satellite position in the future, and to calculate the angles and distances of the satellite.

There are two prediction modes.

The first mode is forecasting by date and time to determine the coordinates of a near-space satellite.

The second mode is forecasting by the coordinates of the observer to determine the date and time of flight of the near-space satellite near the coordinate of the observer, and also in this mode, distances and angles are calculated.

For SPG4 calculations, this program uses satellite TLE files. TLE files for some satellites can be downloaded from the Internet.

This program performs calculations only for near space satellites.

1 Determining the position of the satellite in the future by date and UTC time. To determine the position of the satellite in the future by date and UTC time, it is necessary to add the satellite TLE file by pressing the ADD TLE button. Next, create a text file with dates and UTC time and add it by pressing the ADD TXT button. Then select the path to save the file with the calculation result by pressing the SAVE TXT button and enter the file name. Then press the CALCULATE button.
![изображение](https://user-images.githubusercontent.com/46265118/95046634-35873000-06ed-11eb-984d-e2d065d117e0.png)
An example of an input text file with dates and UTC time and an output text file with the result of the calculation:
![изображение](https://user-images.githubusercontent.com/46265118/95049020-96186c00-06f1-11eb-82ac-c428c48ef72a.png)
2 Determining the date of the satellite's flight, calculating the angles and distances of the satellite. To determine the date and UTC time of the satellite's flight, switch the switch to the position: “Enable calculation by latitude and longitude” and add the satellite TLE file. Then select the path to save the text file with the calculation result by pressing the SAVE TXT button and enter the file name. Next, you need to enter the latitude, longitude of the observer and the time period. Then press the CALCULATE button.
![изображение](https://user-images.githubusercontent.com/46265118/95046792-8c8d0500-06ed-11eb-9e36-ddbba92aff15.png)
The calculation result is a text file with the following calculated values: “Data, Time, Latitude, Longitude, Spacecraft elevation angle, Spacecraft tilt angle, Departure angle from nadir, Distance to spacecraft, Spacecraft height, Azimuth spacecraft, Distance between coordinates, Angle of the sun, Sun azimuth”.
An example of an output text file with the result of the calculation:
![изображение](https://user-images.githubusercontent.com/46265118/95046037-f4dae700-06eb-11eb-9bc9-7de0eaef86e7.png)
![изображение](https://user-images.githubusercontent.com/46265118/95045399-a416be80-06ea-11eb-8ac8-1c5415051f32.png)
![изображение](https://user-images.githubusercontent.com/46265118/95045477-cb6d8b80-06ea-11eb-8559-cc5f870bde85.png)
At the bottom of the text file of the result from a given time period, the minimum angle is highlighted
![изображение](https://user-images.githubusercontent.com/46265118/95218224-bbde6780-07fc-11eb-9373-c9fa266c1170.png)
3 If you need to get more angles, you need to increase the calculation area. To do this, you need to switch the switch: “Enterred Offsets” and enter the offset from latitude and longitude.
![изображение](https://user-images.githubusercontent.com/46265118/95046930-db3a9f00-06ed-11eb-8dc9-fa635abb552e.png)
After clicking on the "Calculate" button, you must wait until the calculation is 100% complete. After 100%, a window will appear informing about the completion of the calculations.
![изображение](https://user-images.githubusercontent.com/46265118/95047178-4ab08e80-06ee-11eb-97e8-bd7a64bb3f1b.png)
System requirements: Windows 10 x86/x64


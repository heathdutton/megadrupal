********************************************************************
                COUNTDOWN TIMER FIELD M O D U L E
********************************************************************
Original Author: Varun Mishra
Current Maintainers: Varun Mishra
Email: varunmishra2006@gmail.com

********************************************************************
DESCRIPTION:

   Countdown timer field module allows you to create countdown timer
   field to count the days, hours, minutes, and seconds until a specified
   event. The module uses jQuery Countdown Timer to display the countdown
   timer in a nice graphical way.


********************************************************************

INSTALLATION:

1. Place the entire field_countdown directory into sites modules directory
  (eg sites/all/modules).

2. Enable this module by navigating to:

     Administration > Modules

3) This module have dependency on date_popup and libraries module.

4) Download jQuery Countdown Timer Library from
   http://tutorialzine.com/2011/12/countdown-jquery/ . Install it in
   sites/all/libraries directory, and rename the directory to jquery-countdown.
   The library should be available at a path like 
   sites/all/libraries/jquery-countdown/assets/countdown/jquery.countdown.js

5) Please read the step by step instructions as an example to use this
   module below:-

a) Install the module. 

b) Go to admin/structure page. Click on manage fields of any content type.

c) Add new field and select "Countdown Timer Field" From Field type. 

d) Now you timer field is ready to use. 

e) Click on Manage Display link to select how do you want to display this field.

f) There are 4 display formats available:-
    I)   jQuery Countdown Timer without text timer.
    II)  jQuery Countdown Timer with text timer.
    III) Date and time as string.
    IV)  Unix time stamp.

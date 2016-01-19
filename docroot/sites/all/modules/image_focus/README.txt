Image Focus Crop module
---

Homepage: http://drupal.org/project/image_focus

== Configuration

1. Use the image_focus_scale_and_crop action

2. Modify Face detection threshold

The PHP implementation is known to be very slow. Images larger than 50 KB will
be ignored. To modify this threshold, please override the variable
'image_focus_face_detection_maxsize'.

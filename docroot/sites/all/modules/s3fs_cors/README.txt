This module is a fork of AmazonS3 CORS Upload, re-written to work with the
S3 File System module, rather than AmazonS3.

===================
    INSTALLATON
===================
You must install the jQuery Update module and set it to use jQuery v1.5 or
higher. Otherwise, the CORS uploads will fail. Note that setting it higher
than 1.8 can cause backwards compatibility problems for other modules'
javascript code.

===================
       SETUP
===================
To configure your S3 bucket so that it will accept CORS uploads, go to the
admin/config/media/s3fs/cors page on your admin site, fill in the "CORS Origin"
field with your site's domain name, and submit it.

To enable the CORS Upload widget, create a new file field (or edit an existing
one) with the "S3 CORS File Upload" widget. This will switch out the usual file
upload mechanism provided by Drupal with one that looks the same, but sends
uploaded files directly to S3, rather than sending them to your Drupal server
first.

===================
   Known Issues
===================
CORS uploading is not supported on older browsers. Your users must use
Internet Explorer 10+ (or Edge), Chrome 30+, Firefox 28+, or Safari 7+.

S3FS CORS also does not yet support multi-value file fields. If you need CORS
upload support for such fields, please post a feature request to the S3FS CORS
issue queue on drupal.org.

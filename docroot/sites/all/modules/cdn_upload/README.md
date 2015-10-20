Installation
=================

 - First clone https://github.com/Widen/fine-uploader into your sites/all/libraries folder so
   that you have a sites/all/libraries/fine-uploader directory.

 - Now navigate to the fine-uploader directory and type the following. Note: You must have Node.js
   installed on the system you wish to perform this build.

      npm install
      grunt

 - You will now need to make sure you meet all the requirements for the awssdk2 module.

      http://drupal.org/project/awssdk2

 - Edit the CORS configuration of your amazon S3 bucket to the following.

      <?xml version="1.0" encoding="UTF-8"?>
      <CORSConfiguration xmlns="http://s3.amazonaws.com/doc/2006-03-01/">
          <CORSRule>
              <AllowedOrigin>*</AllowedOrigin>
              <AllowedMethod>POST</AllowedMethod>
              <AllowedMethod>PUT</AllowedMethod>
              <AllowedMethod>DELETE</AllowedMethod>
              <MaxAgeSeconds>3000</MaxAgeSeconds>
              <ExposeHeader>ETag</ExposeHeader>
              <AllowedHeader>*</AllowedHeader>
          </CORSRule>
      </CORSConfiguration>

 - You will now need to edit the bucket policy for your Amazon S3 bucket to the following.

      {
        "Version": "2008-10-17",
        "Id": "http referer policy example",
        "Statement": [
          {
            "Sid": "readonly policy",
            "Effect": "Allow",
            "Principal": "*",
            "Action": "s3:GetObject",
            "Resource": "arn:aws:s3:::your_bucket_name/*"
          }
        ]
      }

 - After you have all the modules enabled, you will now need to add your Amazon AWS credentials
   by going to /admin/config/aws/service

 - You can then add a Text field to any content type, and then select "S3 Direct Upload" as the
   upload mechanism.  Make sure you provide your Bucket name in the field configurations.

Enjoy.


AWS Amazon module provides easy way to automate backup and delete backups of your Amazon EC / S3 system.

____________________________________________________________________________________________________
Installing the module:
____________________________________________________________________________________________________
1. Delopy and enable the module as always
2. Download and copy the Amazon PHP SDK to sites/all/libraries/AWSSDKforPHP
3. Configure AWS security credentials at Configuration -> System -> Amazon AWS Configuration

The system performs the tasks in Drupal Cron. If you wish to run this more frequently,
use the URL displayed in the configuration screen and schedule your own cron. Note that you'll need to 
flush menu cache everytime you set or change your access key as the URL is generated evertime this changes.

Note:S3 backup is a full backup.

____________________________________________________________________________________________________
File Field Widget:
____________________________________________________________________________________________________
You can use the module to upload the files in any File Field to S3. The files will be automatically
uploaded to S3 and appropriate links generated for usage. When the module is disabled the files
will be restored back on the server. The files are not immediately uploaded to S3. They will be uploaded when
cron runs. To enable this select Structure -> <Content Type> -> Manage Display, select the Format as S3 Files for
the file file. 


Note: When this is enabled, if a file is deleted / removed, you'll find a watchdog entry 

The file !file was not deleted, because it does not exist.

This is not an error. Since Drupal is not able to find the file in file system, it logs the error. Nevertheless
the file will be deleted from S3.

If you are displaying the files in a view using the Views module, with Field as the display format, ensure you 
select the Formatter as S3 Files so that the URLs are generated properly

____________________________________________________________________________________________________
Direct S3 uploads:
____________________________________________________________________________________________________
You can specify buckets to which users can directly upload large files. Each bucket will have an own permission.
Uploads will be saved in the bucket in the path 
direct-uploads/user/[uid]
You can overwrite the upload theme by copying the file aws-amazon-s3-upload.tpl.php in to your active theme folder.
After you add a new bucket, flush the menu cache to generate the upload link.

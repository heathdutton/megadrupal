BASIC INFO
==========

- This module is a sub-cloud system under Cloud module
- It provides almost the functionalities for Amazon,
- OpenStack, and Eucalytpus clouds.
- Additional Amazon based clouds can be added by implementing
- hook_aws_cloud_data().

SYSTEM REQUIREMENTS
===================
- 512MB Memory: If you use aws_cloud module, the running host of this
  system requires more than 512MB memory to download a list of images
 (because it's huge amount of data for listing).
- For OpenStack, use Cactus. 

INSTALLATION: FOR USAGE WITH AMAZON EC2
=======================================
1. Download Libraries Module.
2. Download the AWS SDK for PHP from Amazon: http://pear.amazonwebservices.com/get/sdk-latest.zip
3. Copy the AWS SDK for PHP into sites/all/libraries as awssdk
   (or sites/<your_site_name>/libraries).  If the SDK library is not installed correctly
   the module will not function properly
3. Turn on the Cloud module
4. Turn on the following modules for EC2 management
  - Server Templates
  - AWS Cloud (Found under IaaS package)
  - Pricing
5. In order for the system to automatically update the database with EC2 data, cron must be running.  If cron is not running,
   data can still be imported using the "Refresh page" link at the bottom of listing pages
6. Make sure the sites/default/files directory is writable.
   e.g. If you have /var/www/html/sites/default/files,
      chmod 777 /var/www/html/sites/default/files
      (because it creates files/cloud sub-directory)
   
BASIC USAGE
===========
The following sections describes how to use aws_cloud to manage instances and launch an instance.

1. CONFIGURE AWS CREDENTIALS
============================
1. Download, install and enable all modules as described in the installation instructions
2. Go to admin/config/clouds to add an Amazon EC2 Region.  Click Add Cloud
3. Enter properly EC2 credentials and save
4. Upon successfully saving the credentials, click the link in the success message to refresh all EC2 data
5. Refresh the AWS images by going to Clouds > [YOUR CLOUD] > Images.  Click the "Refresh Page".

2. SETUP SSH KEY
================
Setup a SSH Key for Clanavi to use to execute remote scripts and for SSH access via the SSH Console
1. Go to Clouds > [YOUR CLOUD] > SSH Keys
2. Click "Create" Button
3. Enter the "Key Pair Name"
4. Click "Create"

3. SETUP SERVER TEMPLATE
========================
Launching an EC2 instance requires a configured Server Template.  A server template tells the system the following:
  - AMI to launch (OS choices are packaged as AMIs.  There are many to choose from such as Centos, Ubuntu...etc)
  - Size of the instance (tiny, small, medium, large, #of processors). Detailed description of instances can be found
  here: http://aws.amazon.com/ec2/instance-types/
  - A Server Template can instruct the system to launch multiple instances at the same time
  - The SSH key used with the ROOT user
  - Security group the instance adheres to
  - Zone to launch the server (us-west-1a, us-west-1b...etc)
  
1. Go to Clouds > [YOUR CLOUD] > Launch
2. Click "Create" Button
3. Fill out the "Name" field
4. Fill out the "EC2 Image" field with the AMI to use.  This field will auto-complete to help you locate images
5. Fill out the "User Name".  The "User Name" is used when trying to ssh into the machine via the SSH Console and when a remote script has to be invoked.
   This is usually "root".  For some ubuntu image, this will be "ubuntu"
6. Fill out the "SSH Key"
7. Select a security group.  The default group is sufficient for basic purposes.
8. Fill out the "Count". Count tells the system how many instances to launch.
9. Select "Availablity Zone" if there is a preference.
10. Click "Add" to save the template

4. LAUNCH A SERVER TEMPLATE
===========================
Using the newly created server template, an instance can be launched.

1. Go to Clouds > [YOUR CLOUD] > Launch
2. Find the newly created template
3. Use the "Play" Icon to find the "Launch" command.
4. Click "Launch"
5. A message will inform you of the launch status.  If successful, the cloud listing
page will show the instance state as "Pending".
6. If cron is running, the status will be updated automatically.  If there is no cron running,
click the "Refresh Page"

How to Install OpenStack on Amazon EC2
======================================
1) Launch an 'm1.large' Instance of 
e.g. us-east-1     : ubuntu-natty-11.04-amd64-server-20110426 (ami-68ad5201)

2) Open ports in Security Group
   TCP - IPs: 0.0.0.0/0 Port: 22   (for SSH)
   TCP - IPs: 0.0.0.0/0 Port: 8773 (OpenStack nova API uses 8773) 

3) Log into the instance as a username 'ubuntu'

4) Set up a password for root

sudo passwd root

5) Install bzr - Stay in a directory at /home/ubuntu

sudo apt-get install bzr

6) Download OpenStack nova (Cactus) - Stay in a directory at /home/ubuntu

bzr branch lp:nova/cactus
cp -R cactus nova

7) Install nova - Stay in a directory at /home/ubuntu

sudo nova/contrib/nova.sh install

8) Make or put nova/novarc (root:root)

NOVA_KEY_DIR=$(pushd $(dirname $BASH_SOURCE)>/dev/null; pwd; popd>/dev/null)
export EC2_ACCESS_KEY="admin:admin"
export EC2_SECRET_KEY="admin"
export EC2_URL="http://127.0.0.1:8773/services/Cloud"
export S3_URL="http://127.0.0.1:3333"
export EC2_USER_ID=42 # nova does not use user id, but bundling requires it
export EC2_PRIVATE_KEY=${NOVA_KEY_DIR}/pk.pem
export EC2_CERT=${NOVA_KEY_DIR}/cert.pem
export NOVA_CERT=${NOVA_KEY_DIR}/cacert.pem
export EUCALYPTUS_CERT=${NOVA_CERT} # euca-bundle-image seems to require this set
alias ec2-bundle-image="ec2-bundle-image --cert ${EC2_CERT} --privatekey ${EC2_PRIVATE_KEY} --user 4
2 --ec2cert ${NOVA_CERT}"
alias ec2-upload-bundle="ec2-upload-bundle -a ${EC2_ACCESS_KEY} -s ${EC2_SECRET_KEY} --url ${S3_URL}
 --ec2cert ${NOVA_CERT}"
export NOVA_API_KEY="admin"
export NOVA_USERNAME="admin"
export NOVA_URL="http://127.0.0.1:8774/v1.0/"

9) Run nova - This script will launch 'multiple screens' of screen command.

sudo nova/contrib/nova.sh run

10) Teminate nova 

sudo nova/contrib/nova.sh terminate
sudo nova/contrib/nova.sh clean

or

Ctrl + A D


SSH CONSOLE SETUP
=================

- Change a constant value 'CLOUD_SEED' in ec2_lib_constants.inc to your 
  favorite keyword.
- The security is not considered in current implementation. Please be
  aware that your private key will pass through the Internet if
  you deploy this system on the Internet.
- Go to Cloud on top menue
      | Amazon EC2 (<- the menu will appear as you configured)
      | SSH Keys and edit / update your private key
- If you input your private key through the above menu, the SSH console
  will NOT work.
- You need to get MindTerm from

  http://www.cryptzone.com/products/agmindterm/download.aspx

  and put mindterm.jar file into your <wwwroot> folder.  (e.g. /var/www/html)
  
* NOTE: If you download the open source (FREE) version of MindTerm, you will
  need to sign the jar file.  The commercial version of MindTerm comes signed
  already. If you do not sign the jar file, MindTerm will not have all the
  permissions to make start up properly.  If you try launching the SSH console,
  MindTerm will throw an "access denied" error.  
  
- Launch an Amazon EC2 instance and make sure the instance is running. Then
  click 'console' icon or Console tab in Instance Detail page.
- You need to trust two Java Applets on both mindterm.jar and CloudSSHKeyManager.jar
  by clicking 'Allow' or 'Yes' button in order to handle / transfer your
  private key.
- If you see the following error:

  e.g. Error when setting up authentication:
       C:\Users\<USERNAME>\Application Data\MindTerm\64656661756c74

  then that error message means you skipped CloudSSHKeyManager.jar authentication.
  Please clear Java applet cache from Java control panel and re-launch your
  browser.
- If you are using basic authentication on your site, that would be a problem to
  use this functionality due to the permission.
- Try deleting certificate on Java control panel for refreshing your environment.


MOUDLE DEPENDENCY
=================
This module works with the following modules:
- Cloud
- Server Templates
- Libraries
 

DIRECTORY STRUCTURE FOR AWS MODULE FAMILY
=========================================

cloud/modules/iaas
  +-modules
    o-aws_cloud       (depends on ec2_api        ) (Amazon EC2        is part of AWS)
    x-aws_s3          (depends on s3_api         ) (Amazon S3         is part of AWS)
    x-aws_smpledb     (depends on simpledb_api   ) (Amazon SimpleDB   is part of AWS)
    x-aws_cloud_watch (depends on cloud_watch_api) (Amazon CloudWatch is part of AWS)
      - ...
    x-aws_sqs
      - ...
    x-aws_simpledb
      - ...
    x-aws_cloud_watch
      - ...

x... NOTE: NOT IMPLEMENTED.  SHOWN ONLY AS A REFERENCE AND PROPOSED STRUCTURE SPEC.


LIMITATION
==========
- Some of features are not implemented

Copyright
=========

Copyright (c) 2010-2012 DOCOMO Innovations, Inc.

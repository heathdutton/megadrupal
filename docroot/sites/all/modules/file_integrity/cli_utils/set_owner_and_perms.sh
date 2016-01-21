#! /bin/sh
# Shell script to set ownership and permissions for a Drupal site
# prior to fingerprinting the site with File integrity check.
#
# Make sure you edit the five variables in the configuration section
# to match your site before running it.
#
# You should run this as root.
#
# This is what it does:
# 1. Change all files in and below directory DRUPALROOT to be owned by
#    SITEOWNER and to belong to group WEBSERVERGROUP.
# 2. Set all plain files below DRUPALROOT to have permssion 640 rw-r-----.
# 3. Set all directories below DRUPALROOT to have permssion 750 rwxr-x---.
# 4. Make files in existing upload directories writeable by WEBSERVERGROUP.
#-------------------------------------------------------------------------------

#---( configuration section )---------------------------------------------------
# login username of site owner:
SITEOWNER=joe
# the group the web-server belong to (usually apache or www-data)
WEBSERVERGROUP=apache
# The path from the file system root to the Drupal root
DRUPALROOT=/var/www/html
# The path from the file system root to the public upload area.
PUBLIC=${DRUPALROOT}/sites/default/files/
# The path from the file system root to the private upload area.
PRIVATE=${DRUPALROOT}/private
#-------------------------------------------------------------------------------

# First some sanity checks.
id -u ${SITEOWNER} > /dev/null 2> /dev/null
if [ $? -ne 0 ]
then
  echo The user \"${SITEOWNER}\" does not exist. Aborting.
  exit
fi
id -g ${WEBSERVERGROUP} > /dev/null 2> /dev/null
if [ $? -ne 0 ]
then
  echo The group \"${WEBSERVERGROUP}\" does not exist. Aborting.
  exit
fi
if [ ! -d "${DRUPALROOT}" ]
then
  echo The directory \"${DRUPALROOT}\" does not exist. Aborting.
  exit
fi
if [ -n "${PUBLIC}" -a ! -d "${PUBLIC}" ]
then
  echo The directory \"${PUBLIC}\" does not exist. Aborting.
  exit
fi
if [ -n "${PRIVATE}" -a ! -d "${PRIVATE}" ]
then
  echo The directory \"${PRIVATE}\" does not exist. Aborting.
  exit
fi

# 1. Make all files owned by SITEOWNER.WEBSERVERGROUP.
chown -R ${SITEOWNER}.${WEBSERVERGROUP} ${DRUPALROOT}/

# 2. Set all plain files below DRUPALROOT to have permssion 640 rw-r-----.
cd ${DRUPALROOT} 
chmod 640 $(find . -type f)

# 3. Set all directories below DRUPALROOT to have permssion 750 rwxr-x---.
cd ${DRUPALROOT} 
chmod 750 $(find . -type d)

# 4. Make files in existing upload directories writeable by WEBSERVERGROUP.
if [ -n "${PUBLIC}" -a -d "${PUBLIC}" ]
then
  chmod -R g+w ${PUBLIC}
fi
if [ -n "${PRIVATE}" -a -d "${PRIVATE}" ]
then
  chmod -R g+w ${PRIVATE}
fi
echo File ownership and permissions for site have been set.
# EOF

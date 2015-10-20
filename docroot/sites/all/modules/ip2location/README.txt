Description:
IP2Location is a non-intrusive geo IP solution to help you to identify
visitor's geographical location, i.e. country, region, city, latitude,
longitude, ZIP code, time zone, connection speed, ISP and domain name, IDD
country code, area code, weather station code and name, and mobile carrier,
elevation, usage type information using a proprietary IP address lookup
database and technology without invading the Internet user's privacy.

This extension enables geolocation data in server variables in real time. Your
can use these variables in other modules or themes. IP2Location extension comes
with empty database. Please download a free database from
http://lite.ip2location.com or commercial version from
http://www.ip2location.com.


Requirements:
Drupal 7.x


Installation:
1. Copy the extracted ip2location directory to your Drupal sites/all/modules
directory.
2. Login as an administrator. Enable the module at
http://www.example.com/?q=admin/modules
3. Set your private file system path at
http://www.example.com/?q=admin/config/media/file-system
4. Configure IP2Location database path at
http://www.example.com/?q=admin/config/people/ip2location.


Usage:
1. The IP2Location variables are listed as below, you and access the variables
within any modules or pages.

$_SERVER['IP2LOCATION_IP_ADDRESS']: Visitor IP address.
$_SERVER['IP2LOCATION_COUNTRY_CODE']: Two-character country code based on ISO
3166.
$_SERVER['IP2LOCATION_COUNTRY_NAME']: Country name based on ISO 3166.
$_SERVER['IP2LOCATION_REGION_NAME']: Region or state name.
$_SERVER['IP2LOCATION_CITY_NAME']: City name.
$_SERVER['IP2LOCATION_LATITUDE']: Latitude of city.
$_SERVER['IP2LOCATION_LONGITUDE']: Longitude of city.
$_SERVER['IP2LOCATION_ISP']: Internet Service Provider or company's name.
$_SERVER['IP2LOCATION_DOMAIN_NAME']: Internet domain name associated to IP
address range.
$_SERVER['IP2LOCATION_ZIP_CODE']: ZIP/Postal code.
$_SERVER['IP2LOCATION_TIME_ZONE']: UTC time zone.
$_SERVER['IP2LOCATION_NET_SPEED']: Internet connection type.
$_SERVER['IP2LOCATION_IDD_CODE']: The IDD prefix to call the city from another
country.
$_SERVER['IP2LOCATION_AREA_CODE']: A varying length number assigned to
geographic areas for call between cities.
$_SERVER['IP2LOCATION_WEATHER_STATION_CODE']: The special code to identify the
nearest weather observation station.
$_SERVER['IP2LOCATION_WEATHER_STATION_NAME']: The name of the nearest weather
observation station.
$_SERVER['IP2LOCATION_MCC']: Mobile Country Codes (MCC) as defined in ITU E.212
for use in identifying mobile stations in wireless telephone networks,
particularly GSM and UMTS networks.
$_SERVER['IP2LOCATION_MNC']: Mobile Network Code (MNC) is used in combination
with a Mobile Country Code (MCC) to uniquely identify a mobile phone operator
or carrier.
$_SERVER['IP2LOCATION_CARRIER_NAME']: Commercial brand associated with the
mobile carrier.
$_SERVER['IP2LOCATION_ELEVATION']: Average height of city above sea level in
meters (m).
$_SERVER['IP2LOCATION_USAGE_TYPE']: Usage type classification of ISP or company


Support:
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/ip2location

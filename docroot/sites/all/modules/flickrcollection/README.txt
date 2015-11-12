The FlickrCollection module is an add-on module for the FlickrGallery
module.

The FlickrGallery module let you specify which sets you want to pull
from Flickr. Unfortunately you will have to add all the Flickr Set
ID's manually.

The FlickrCollection module will let you add a Flickr Collection ID
and then keeps the FlickrGallery module updated with the sets in the
specified collection.

Remember to run cron to have the collection of sets updated if it
changes over time.

# Configuration

Just visit admin/config/media/flickrcollection and enter the
Flickr Collection ID.

Make sure to setup and configure the FlickrGallery module and
FlickrAPI module with your Flickr credentials.

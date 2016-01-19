## Setup

Go to example.com/admin/config/media/ng-lightbox and configure the paths you would
like to be auto-lightboxed.

Any page that runs through theme_link() and has either the internal path or alias listed at
/admin/config/media/ng-lightbox will automatically be lightboxed. If you want to manually apply the lightbox to a link,
simply add the "ng-lightbox" class to the anchor.

## Client Side Integration

There maybe times when you can not add the ng-lightbox class to the element server side, such as within a map. You can
setup an NG Lightbox client side like so:

    NGLightbox.initElement(element, path);

Note, the path parameter provides the URL to load into the lightbox. If element is an anchor then the path will be read
from the href attribute if not provided.

## Rebuilding SASS

If you're contributing to ng_lightbox you can re-generate the css with the
following command from within the ng_lightbox folder.

    compass compile --no-sourcemap --no-debug-info --force -e production sass/lightbox.scss

If you're simply using the ng_lightbox module and want to have your own theme you can copy the
sass file into your theme and begin customising from there.

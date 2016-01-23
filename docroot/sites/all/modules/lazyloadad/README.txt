jQuery LazyLoad Ad (http://jqueryad.web2ajax.fr/) is a great jQuery script, 
that allows us to lazy load ads on a page.

A lot of ad providers use embed codes, that are based on document.write(),
which can cause a lot of problems. A common solution to this problem is to 
load ad in an iFrame, but is not always possible to implement this 
(streching ads are one of the cases).

jQuery LazyLoad Ad will allow you to still embed your ads inline, but will
delay their display until site was completely loaded. Even more; ads will 
not be loaded until actually visible on page by default.

-- Installation --

Download module and enable it. 

Then navigate to admin/config/system/lazyloadad and configure jQuery
selectors for your ads. Ads must be modified to work with this script. 

A typical AdSense embed code would look like this:

      <script type="text/javascript"><!--
          google_ad_client = "pub-8333082972366661";
          /* 300x250, date de création 18/03/10 */
          google_ad_slot = "9789857825";
          google_ad_width = 300;
          google_ad_height = 250;
          //-->
      </script>
      <script type="text/javascript" src="http://pagead2.googlesyndication.com/pagead/show_ads.js"></script>

The same code, rewritter to be used will look like this:

      <div class="my_ad" original="http://pagead2.googlesyndication.com/pagead/show_ads.js">
          <code type="text/javascript"><!--
              google_ad_client = "pub-8333082972366661";
              google_ad_slot = "9789857825";
              google_ad_width = 300;
              google_ad_height = 250;
              //-->
          </code>
      </div>

See jQuery LazyLoad Ad docs (http://jqueryad.web2ajax.fr/doc/) for more info.


-- Maintainer --
Janez Urevc (slashrsm) - http://drupal.org/user/744628
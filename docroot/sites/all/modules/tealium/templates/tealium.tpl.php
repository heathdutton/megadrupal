<?php

use Drupal\tealium\TealiumUtagData;
use Drupal\tealium\Data\TealiumJqueryEventBinding;

/**
 * @file
 * Theme Tealium tags.
 *
 * @var string $account
 *  The Tealium account.
 * @var string $profile
 *  The Tealium profile.
 * @var string $environment
 *  The Tealium environment [dev|qa|prod].
 *
 * @var string $utag_async
 *  The JavaScript value for the Universal Data Tag async property. Will be
 *  either set to 'true' or 'false'.
 *
 * @var TealiumUtagData|null $data_variables
 *  Variables to be set in the Tealium universal data object.
 * @var TealiumUtagData|null $view_variables
 *  Variables to be sent to Tealium as a 'view' event on page load.
 * @var TealiumUtagData|null $link_variables
 *  Variables to be sent to Tealium as a 'link' event on page load.
 * @var TealiumJqueryEventBinding[] $bind_variables
 *  Variables to be sent to Tealium as a tracking event, when a DOM element event fires.
 *
 * @var string|null $data_variables_js
 *  Tealium Universal Data Object variables for the page as JavaScript.
 * @var string|null $view_variables_js
 *  Tealium view variables for the page as JavaScript.
 * @var string|null $link_variables_js
 *  Tealium link variables for the page as JavaScript.
 * @var string|null $bind_variables_js
 *  JavaScript to send Tealium variables as a tracking event, when a DOM element event fires.
 *
 * @link http://wiki.tealiumiq.com/index.php/Data_Object
 * @link http://wiki.tealiumiq.com/index.php/AJAX/Flash
 *
 * @see template_preprocess_tealium
 *
 * @ingroup themeable
 */
?>
<?php if ($account && $profile && $environment) : ?>
<!-- Begin Tealium Integration -->
<?php if (!empty($data_variables_js)) : ?>

  <script type="text/javascript">
    <!--//--><![CDATA[//><!--
    var utag_data = <?php print $data_variables_js; ?>;
    //--><!]]>
  </script>
<?php endif; ?>

  <script type="text/javascript">
    <!--//--><![CDATA[//><!--
    (function(a,b,c,d){
      a='//tags.tiqcdn.com/utag/<?php print $account; ?>/<?php print $profile; ?>/<?php print $environment; ?>/utag.js';
      b=document;c='script';d=b.createElement(c);d.src=a;d.type='text/java'+c;d.async=<?php print $utag_async; ?>;
      a=b.getElementsByTagName(c)[0];a.parentNode.insertBefore(d,a);
    })();
    //--><!]]>
  </script>

  <?php if (!empty($link_variables_js) || !empty($view_variables_js) || !empty($bind_variables_js)) : ?>

  <script type="text/javascript">
    <!--//--><![CDATA[//><!--
    <?php if (!empty($link_variables_js) || !empty($view_variables_js)) : ?>

    (function ($, u) {
      $(document).ready(function () {
        <?php if (!empty($link_variables_js)) : ?>

        u.track("link", <?php print $link_variables_js; ?>);
        <?php endif; ?>

        <?php if (!empty($view_variables_js)) : ?>

        u.track("view", <?php print $view_variables_js; ?>);
        <?php endif; ?>

      });
    }(jQuery, utag));
    <?php endif; ?>

    <?php if (!empty($bind_variables_js)) : ?>

      <?php print $bind_variables_js; ?>
    <?php endif; ?>

    //--><!]]>
  </script>
  <?php endif; ?>

<!-- End Tealium Integration -->
<?php endif; ?>

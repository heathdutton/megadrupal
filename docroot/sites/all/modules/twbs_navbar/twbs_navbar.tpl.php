<?php

/**
 * @file
 * Default template for twbs_navbar.
 *
 * Available variables:
 * - $navbar_brand: Link for brand.
 * - $navbar_nav: All menu links.
 *
 * @see template_preprocess()
 * @see template_preprocess_twbs_navbar()
 *
 * @ingroup themeable
 */
?>
<div class="twbs-navbar navbar navbar-inverse navbar-fixed-top" role="navigation">
    <div class="container">
        <div class="navbar-header">
            <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".twbs-navbar .navbar-collapse">
                <span class="sr-only">Toggle navigation</span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
            </button>
            <?php print $navbar_brand; ?>
        </div>
        <div class="navbar-collapse collapse">
            <?php print $navbar_nav; ?>
        </div> <!-- /.nav-collapse -->
    </div>
</div>

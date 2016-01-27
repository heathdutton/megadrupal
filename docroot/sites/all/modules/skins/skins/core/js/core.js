(function ($) {

Drupal.behaviors.skinsHasJS = {
  attach: function (context, settings) {
    $('html').removeClass('no-js');
  }
};

Drupal.behaviors.skinsfusionEqualheights = {
  attach: function (context, settings) {
    if (jQuery().equalHeights) {
      $("#header-top-wrapper div.equal-heights div.content").equalHeights();
      $("#header-group-wrapper div.equal-heights div.content").equalHeights();
      $("#preface-top-wrapper div.equal-heights div.content").equalHeights();
      $("#preface-bottom div.equal-heights div.content").equalHeights();
      $("#sidebar-first div.equal-heights div.content").equalHeights();
      $("#content-region div.equal-heights div.content").equalHeights();
      $("#node-top div.equal-heights div.content").equalHeights();
      $("#node-bottom div.equal-heights div.content").equalHeights();
      $("#sidebar-second div.equal-heights div.content").equalHeights();
      $("#postscript-top div.equal-heights div.content").equalHeights();
      $("#postscript-bottom-wrapper div.equal-heights div.content").equalHeights();
      $("#footer-wrapper div.equal-heights div.content").equalHeights();
    }
  }
};

Drupal.behaviors.skinsomegaEqualheights = {
  attach: function (context, settings) {
    if (jQuery().equalHeights) {
      $("#zone-user-wrapper div.equal-heights div.content").equalHeights();
      $("#zone-branding-wrapper div.equal-heights div.content").equalHeights();
      $("#zone-menu-wrapper div.equal-heights div.content").equalHeights();
      $("#zone-header-wrapper div.equal-heights div.content").equalHeights();
      $("#zone-preface-wrapper div.equal-heights div.content").equalHeights();
      $("#region-sidebar-first div.equal-heights div.content").equalHeights();
      $("#region-content div.equal-heights div.content").equalHeights();
      $("#region-sidebar-last div.equal-heights div.content").equalHeights();
      $("#zone-postscript-wrapper div.equal-heights div.content").equalHeights();
      $("#region-footer-first div.equal-heights div.content").equalHeights();
      $("#region-footer-second div.equal-heights div.content").equalHeights();
    }
  }
};


Drupal.behaviors.skinsOverlabel = {
  attach: function (context, settings) {
    if (jQuery().overlabel) {
      $("div.skins-horiz-login label").overlabel();
    }
  }
};

})(jQuery);
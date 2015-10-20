(function ($) {
  "use strict";
  Drupal.behaviors.brandseven = {};
  Drupal.behaviors.brandseven.attach = function (context) {
    if($("ul.action-links a i").length == ''){
      $("ul.action-links a").prepend('<i class="icon-plus"></i> ');
    }
    if($("ul.admin-list li i").length == ''){
      $("ul.admin-list li").prepend('<i class="icon-right-open"></i> ');
    }
    if(!$("div.add-or-remove-shortcuts span.icon").hasClass('icon-plus')){
      $("div.add-or-remove-shortcuts span.icon").addClass('icon-plus');
    }
  }
}(jQuery));

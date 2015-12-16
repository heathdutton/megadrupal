/*jshint strict:true, browser:true, curly:true, eqeqeq:true, expr:true, forin:true, latedef:true, newcap:true, noarg:true, trailing: true, undef:true, unused:true */
/*global Drupal: true, jQuery: true, MultiFile_fields: true*/
(function ($) {
  "use strict";
  Drupal.behaviors.webform_multifile = {
    attach: function () {

      // Add the extra brakets to all the Multifile fields
      $("input[type=file].multi").once(function(){
        var $field = $(this);
        var newname = ($field.attr('name') + '[]');
        if ($.browser.msie === true) {
          $field.replaceWith($($field.get(0).outerHTML.replace(/name=\w+/ig, 'name=' + newname)));
        }
        else {
          $field.attr('name', newname);
        }
      });

      // Re-initialize multifile fields with the proper settings
      if(typeof MultiFile_fields !== 'undefined'){
        for(var i=0; i<MultiFile_fields.length; i++){
          $('input[type=file].multi.'+MultiFile_fields[i].id).MultiFile(MultiFile_fields[i].properties);
        }
      }
    }
  };
} (jQuery));

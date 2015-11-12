(function ($) {
  
  Drupal.behaviors.reservationsAddMore = {
  attach: function (context, settings) {
  
    $('#edit-reservations-more', context).click(function() {
     
      //this could be MUCH tighter by looping through an array, 
      //but not sure what's happening with this yet
      if ($('#edit-reservations-reservation-items-choice-3-reservations-item-nid').val()){
        $('.form-item-reservations-reservation-items-choice-4-reservations-item-nid').parent().parent().show();
        $('.form-item-reservations-reservation-items-choice-4-reservations-item-nid').show();
        $('.form-item-reservations-reservation-items-choice-5-reservations-item-nid').parent().parent().show();
        $('.form-item-reservations-reservation-items-choice-5-reservations-item-nid').show();
        $('.form-item-reservations-reservation-items-choice-6-reservations-item-nid').parent().parent().show();
        $('.form-item-reservations-reservation-items-choice-6-reservations-item-nid').show();
      }

      if ($('#edit-reservations-reservation-items-choice-6-reservations-item-nid').val()){
        $('.form-item-reservations-reservation-items-choice-7-reservations-item-nid').parent().parent().show();
        $('.form-item-reservations-reservation-items-choice-7-reservations-item-nid').show();
        $('.form-item-reservations-reservation-items-choice-8-reservations-item-nid').parent().parent().show();
        $('.form-item-reservations-reservation-items-choice-8-reservations-item-nid').show();
        $('.form-item-reservations-reservation-items-choice-9-reservations-item-nid').parent().parent().show();
        $('.form-item-reservations-reservation-items-choice-9-reservations-item-nid').show();
      }

      if ($('#edit-reservations-reservation-items-choice-9-reservations-item-nid').val()){
        $('.form-item-reservations-reservation-items-choice-10-reservations-item-nid').parent().parent().show();
        $('.form-item-reservations-reservation-items-choice-10-reservations-item-nid').show();
        $('.form-item-reservations-reservation-items-choice-11-reservations-item-nid').parent().parent().show();
        $('.form-item-reservations-reservation-items-choice-11-reservations-item-nid').show();
        $('.form-item-reservations-reservation-items-choice-12-reservations-item-nid').parent().parent().show();
        $('.form-item-reservations-reservation-items-choice-12-reservations-item-nid').show();
      }
    });
    
  }};
  
}) (jQuery);

jQuery(document).ready(function($) {

  if ($('.form-item-reservations-reservation-items-choice-4-reservations-item-nid').is(':hidden')) {
    $('.form-item-reservations-reservation-items-choice-4-reservations-item-nid').parent().parent().hide();
  }

  if ($('.form-item-reservations-reservation-items-choice-5-reservations-item-nid').is(':hidden')) {
    $('.form-item-reservations-reservation-items-choice-5-reservations-item-nid').parent().parent().hide();
  }

  if ($('.form-item-reservations-reservation-items-choice-6-reservations-item-nid').is(':hidden')) {
    $('.form-item-reservations-reservation-items-choice-6-reservations-item-nid').parent().parent().hide();
  }

  if ($('.form-item-reservations-reservation-items-choice-7-reservations-item-nid').is(':hidden')) {
    $('.form-item-reservations-reservation-items-choice-7-reservations-item-nid').parent().parent().hide();
  }

  if ($('.form-item-reservations-reservation-items-choice-8-reservations-item-nid').is(':hidden')) {
    $('.form-item-reservations-reservation-items-choice-8-reservations-item-nid').parent().parent().hide();
  }

  if ($('.form-item-reservations-reservation-items-choice-9-reservations-item-nid').is(':hidden')) {
    $('.form-item-reservations-reservation-items-choice-9-reservations-item-nid').parent().parent().hide();
  }

  if ($('.form-item-reservations-reservation-items-choice-10-reservations-item-nid').is(':hidden')) {
    $('.form-item-reservations-reservation-items-choice-10-reservations-item-nid').parent().parent().hide();
  }

  if ($('.form-item-reservations-reservation-items-choice-11-reservations-item-nid').is(':hidden')) {
    $('.form-item-reservations-reservation-items-choice-11-reservations-item-nid').parent().parent().hide();
  }

  if ($('.form-item-reservations-reservation-items-choice-12-reservations-item-nid').is(':hidden')) {
    $('.form-item-reservations-reservation-items-choice-12-reservations-item-nid').parent().parent().hide();
  }
  
  if ($('#edit-reservations-reservation-items-choice-3-reservations-item-nid').val()){
    $('.form-item-reservations-reservation-items-choice-4-reservations-item-nid').parent().parent().show();
    $('.form-item-reservations-reservation-items-choice-4-reservations-item-nid').show();
    $('.form-item-reservations-reservation-items-choice-5-reservations-item-nid').parent().parent().show();
    $('.form-item-reservations-reservation-items-choice-5-reservations-item-nid').show();
    $('.form-item-reservations-reservation-items-choice-6-reservations-item-nid').parent().parent().show();
    $('.form-item-reservations-reservation-items-choice-6-reservations-item-nid').show();
  }

  if ($('#edit-reservations-reservation-items-choice-6-reservations-item-nid').val() > 0){
    $('.form-item-reservations-reservation-items-choice-7-reservations-item-nid').parent().parent().show();
    $('.form-item-reservations-reservation-items-choice-7-reservations-item-nid').show();
    $('.form-item-reservations-reservation-items-choice-8-reservations-item-nid').parent().parent().show();
    $('.form-item-reservations-reservation-items-choice-8-reservations-item-nid').show();
    $('.form-item-reservations-reservation-items-choice-9-reservations-item-nid').parent().parent().show();
    $('.form-item-reservations-reservation-items-choice-9-reservations-item-nid').show();
  }

  if ($('#edit-reservations-reservation-items-choice-9-reservations-item-nid').val() > 0){
    $('.form-item-reservations-reservation-items-choice-10-reservations-item-nid').parent().parent().show();
    $('.form-item-reservations-reservation-items-choice-10-reservations-item-nid').show();
    $('.form-item-reservations-reservation-items-choice-11-reservations-item-nid').parent().parent().show();
    $('.form-item-reservations-reservation-items-choice-11-reservations-item-nid').show();
    $('.form-item-reservations-reservation-items-choice-12-reservations-item-nid').parent().parent().show();
    $('.form-item-reservations-reservation-items-choice-12-reservations-item-nid').show();
  }
});
(function ($) {
  /** Extend ajax commands for update cart block **/
  Drupal.ajax.prototype.commands.commerce_ajax_cart_update = function (ajax, response, status ) {
    window.clearTimeout(Drupal.behaviors.commerce_add_to_cart_show_ajax_cart.timer);
    $.post(Drupal.settings.commerce_ajax_cart.update_url,function(data) {
      jQuery('.view-id-shopping_cart').replaceWith(data) ;
      /** Reattach behaviour **/
      container = jQuery('.view-id-shopping_cart').parent();
      container.unbind('mouseenter').unbind('mouseleave') ;
      Drupal.behaviors.commerce_add_to_cart_show_ajax_cart.attach(container);
      Drupal.behaviors.commerce_add_to_cart_show_ajax_cart.fillCartBlock() ;
      if ( jQuery('.commerce-add-to-cart-confirmation').length > 0 ) {
        var options = { "my" : "center center" , "at" : "center center" , "of" : jQuery(window) };
        jQuery('.commerce-add-to-cart-confirmation').position(options) ;
      }
    }) ;
  }
  Drupal.behaviors.commerce_add_to_cart_show_ajax_cart = {
   timer : 0 ,
   delay : 500,
   fillCartBlock : function() {
     jQuery('a.commerce-ajax-cart-loader').each(function(){
       $.post(Drupal.settings.commerce_ajax_cart.update_url_block,function(data) {
         jQuery('a.commerce-ajax-cart-loader').html(data);
       }) ;
     })
   },
   repositioning : function() {
     var options = {
         "my"        : Drupal.settings.commerce_ajax_cart.position.my,
         "at"        : Drupal.settings.commerce_ajax_cart.position.at,
         "of"        : $('.view-id-shopping_cart').parent(),
         "collision" : Drupal.settings.commerce_ajax_cart.position.collision
       };
     jQuery('#commerce-ajax-cart-preview').position(options) ;
   },
  	attach:function (context, settings) {
     /** Call for chached sites to update block display **/
  	 var container = jQuery(context).find('.view-id-shopping_cart').parent(); 
     jQuery(container).once('commerce-ajax-cart-processed', function() {
       Drupal.ajax.prototype.commands.commerce_ajax_cart_update();
       Drupal.behaviors.commerce_add_to_cart_show_ajax_cart.fillCartBlock(context) ;
     });
     jQuery('#dc-cart-ajax-form-wrapper form').once('commerce-ajax-cart-update',function(){
       jQuery(this).find('a').bind('click',function(){
         Drupal.ajax.prototype.commands.commerce_ajax_cart_update();
       })
     })
     
   		container.bind('mouseenter', function(e) {
   		  window.clearTimeout(Drupal.behaviors.commerce_add_to_cart_show_ajax_cart.timer);
       e.preventDefault();  

       /** Check if preview div is alread created **/
       if ( jQuery('#commerce-ajax-cart-preview').length > 0 ) return ;
       
       /** Append div container **/
       jQuery(this).append('<div class="loading" id="commerce-ajax-cart-preview"><span>Loading</</div>');

       /** Set position for cart preview with jquery.ui.position **/
       //jQuery('#commerce-ajax-cart-preview').position(options) ;
       Drupal.behaviors.commerce_add_to_cart_show_ajax_cart.repositioning() ;
       
       /** Update cart preview **/
       $.post(Drupal.settings.commerce_ajax_cart.ajax_url,function(data) {
         jQuery('#commerce-ajax-cart-preview').removeClass('loading').html(data);
         Drupal.behaviors.commerce_add_to_cart_show_ajax_cart.repositioning() ;
       }) ;
       
     }).bind('mouseleave',function(){
       /** Bind mousehandler for close cart preview **/
       window.clearTimeout(Drupal.behaviors.commerce_add_to_cart_show_ajax_cart.timer);
     	 Drupal.behaviors.commerce_add_to_cart_show_ajax_cart.timer = window.setTimeout(function(){
     	   jQuery('#commerce-ajax-cart-preview').remove();
     	 },Drupal.behaviors.commerce_add_to_cart_show_ajax_cart.delay);
     });
   }
  }
})(jQuery);

Drupal.ajax.prototype.commands.commerceAjaxCartFireTrigger = function(ajax, response, status) {
	jQuery(window).trigger('commerce_ajax_cart_update',response.data)
}
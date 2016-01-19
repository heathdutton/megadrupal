jQuery(document).ready(function ($) {
    $(window).load(function() {
    $('#orbitDemo').orbit({
         animation: 'horizontal-push',
         bullets: true,
     });
     });

   
    $('#slideshow').mouseover(function() {$('.slider-nav span.right').show()});
    $('#slideshow').mouseover(function() {$('.slider-nav span.left').show()});
    $('.slider-nav span.right').mouseover(function() {$('.slider-nav span.right').show()});
    $('.slider-nav span.left').mouseover(function() {$('.slider-nav span.left').show()});
    $('#slideshow').mouseleave(function() {$('.slider-nav span.right').hide()});
    $('#slideshow').mouseleave(function() {$('.slider-nav span.left').hide()});    
     });    

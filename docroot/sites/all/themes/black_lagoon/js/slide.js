jQuery(document).ready(function ($) {
    $(window).load(function() {
    $('#orbitDemo').orbit({
         animation: 'fade',
         bullets: true,
     });
     });

   
    $('.slider').mouseover(function() {$('.slider-nav span.right').show()});
    $('.slider').mouseover(function() {$('.slider-nav span.left').show()});
    $('.slider-nav span.right').mouseover(function() {$('.slider-nav span.right').show()});
    $('.slider-nav span.left').mouseover(function() {$('.slider-nav span.left').show()});
    $('.slider').mouseleave(function() {$('.slider-nav span.right').hide()});
    $('.slider').mouseleave(function() {$('.slider-nav span.left').hide()});    
     });    
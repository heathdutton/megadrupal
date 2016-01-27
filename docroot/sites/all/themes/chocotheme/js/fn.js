jQuery(function ($) {
    $('.commentlist > li:last-child').addClass('last');
});

jQuery(function($) {
jQuery.support.placeholder = false;
WebKit_type_browser = document.createElement('input');
jQuery.support.placeholder = ('placeholder' in WebKit_type_browser);

if(!$.support.placeholder) {
var active = document.activeElement;
$(':text, textarea, :password').focus(function () {
if ($(this).attr('placeholder') != ' ' && $(this).val() == $(this).attr('placeholder')) {
$(this).val('').removeClass('hasPlaceholder');
}
}).blur(function () {
if ($(this).attr('placeholder') != '' && ($(this).val() == '' || $(this).val() == $(this).attr('placeholder'))) {
$(this).val($(this).attr('placeholder')).addClass('hasPlaceholder');
}
});
$(':text, textarea, :password').blur();
$(active).focus();
$('form').submit(function () {
$(this).find('.hasPlaceholder').each(function() { $(this).val(''); });
});
}

});

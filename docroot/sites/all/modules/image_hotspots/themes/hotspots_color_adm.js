(function($) {
  $(document).ready(function() {
    for (var i = 1; i <= 12; i++) {
	  $('.hotspots_example').append($('<div />', {'class': 'hotspots_example_area hotspots_example_' + i, 'data-hotspots-num': i}));
	}
	$('.hotspots_example').append($('<div />', {'class': 'hotspots_refresh'}).click(function () {hotspotRefresh();}));
	hotspotRefresh();
  });

  function hotspotRefresh() {
    var backgroundColor;
    var borderStyle;
    var borderWidth;
    var borderColor;
    var opacity;
    var num;
    $rows = $('.fieldset-wrapper:visible');
    $areas = $('.hotspots_example_area');
    for (var i = 1; i <= $areas.length; i++) {
      num = i % $rows.length;
      if (num == 0) {
      	num = $rows.length;
      }
      backgroundColor = (jQuery('.hotspot_color_field', $rows[num-1]).val() == '') ? 'transparent' : '#' + jQuery('.hotspot_color_field', $rows[num-1]).val();
      borderStyle = jQuery('.hotspot_border_style_field', $rows[num-1]).val();
      borderWidth = parseInt(jQuery('.hotspot_border_width_field', $rows[num-1]).val(), 10);
      borderColor = jQuery('.hotspot_border_color_field', $rows[num-1]).val();
      if (borderWidth != 'NaN' && borderColor != '') {
        border = borderWidth + 'px ' + borderStyle + ' #' + borderColor;
      }
      else {
      	border = 'none';
      }
      opacity = parseFloat(jQuery('.hotspot_opacity_field', $rows[num-1]).val());
      opacity = (opacity == 'NaN') ? '0.3' : opacity;
      $('.hotspots_example_' + i)
        .css('background-color', backgroundColor)
        .css('opacity', opacity)
        .css('border', border);
    }
  }

})(jQuery);

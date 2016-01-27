jQuery(function() {
  jQuery('img[placeholder-data]').hover(function() {
    var $this = jQuery(this);
    $this
      .attr('tmp', $this.attr('src'))
      .attr('src', $this.attr('placeholder-data'))
      .attr('placeholder-data', $this.attr('tmp'))
      .removeAttr('tmp');
  }).each(function() {
    jQuery('<img />').attr('src', jQuery(this).attr('placeholder-data'));
  });;
});
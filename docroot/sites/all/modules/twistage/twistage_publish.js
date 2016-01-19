function addVideo(a) {
  $(a).hide();
  $(a).parents('fieldset').clone().removeClass('twistage-add-original').appendTo('#twistage-add-replicate').find('input, textarea, select').each(function(i) {
    var str = this.name;
    m = str.match(/^twistage_upload\[(\d)\]\[([a-z_]+)\]$/);
    if(m) {
      var seq = (parseInt(m[1])+1);
      this.name = 'twistage_upload['+seq+']['+ m[2]+']';
    }
    n = str.match(/^files\[twistage_upload_(\d)\]$/);
    if(n) {
      this.name = 'files[twistage_upload_'+(parseInt(n[1])+1)+']';
    }
  }).attr('value', '').end().find('.twistage-add-more').show().end().find('.form-checkbox').val('value', '1');
  $('.twistage-add-more').click(function() { 
    addVideo(this);
  });
  var i = (parseInt($('#edit-twistage-sequence').val())+1);
  $('#edit-twistage-sequence').val(i);
}

$(document).ready(function(){
  // Drupal (or, at least, D5) does not seem to allow multiple file upload fields in the same form tree, so this a workaround.
  $('#twistage-add-replicate input.form-file').each(function(i) {
    $(this).attr('name', 'files[twistage_upload_'+i+']');
  });

	$('.twistage-add-more').click(function() {
	  addVideo(this);
	});
});
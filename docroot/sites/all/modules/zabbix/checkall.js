(function($){
    // Check All checkboxes
    $(function () {
	$('#edit-zabbix-checkall-enabled').click(function () {
	    var checked_status = this.checked;
	    $(':parent(form) :checkbox').each(function() {
		this.checked = checked_status;
	    });
	});
    });

    // When select form element changes, find all other select elements
    // and set to the original select option value.
    $(function () {
	$('#edit-zabbix-checkall-run-interval').change(function () {
	    var option = $('#edit-zabbix-checkall-run-interval :selected').val();
	    $(':parent(form) select').val(option);
	});
    })
})(jQuery);

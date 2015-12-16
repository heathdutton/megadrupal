/**
 * @file Plugin to cleanup pasted text.
 */
CKEDITOR.plugins.add( 'pasteformat',
{
	init : function( editor )
	{
		function pasteformat_cleanup( ev )
		{
			/*console.log(ev.data);*/
			if (ev.data.html || ev.data.dataValue) {
				jQuery.ajax({
					type: 'POST',
					url: Drupal.settings.basePath + 'pasteformat/ajax/pasteformat_cleanup',
					async: false,
					data: {'html': ev.data.html || ev.data.dataValue},
					success: function(data) {
						/*console.log(data);*/
						/*D6: need to turn returned string to object*/
						if (typeof data != 'object') {
							data = eval('(' + data + ')');
						}
						if (ev.data.html) {
							ev.data.html = data.text;
						}
						else if (ev.data.dataValue) {
							ev.data.dataValue = data.text;
						}
						if (data.alert) {
							alert(data.alert);
						}
					},
					error: function(msg) {
						alert("Failed clean-up on paste, please contact your site administrator.");
					}
				});
			}
			/*console.log(ev.data);*/
		}
		
		editor.on( 'paste', pasteformat_cleanup );
	}
});

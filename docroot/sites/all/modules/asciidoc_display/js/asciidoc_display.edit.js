/**
 * @file
 * JavaScript for editing AsciiDoc.
 *
 * Thanks: https://github.com/lordofthejars/asciidoctor-markitup
 */

(function ($) {
    $(document).ready(function() {
        $('.mark-it-up').markItUp({
            previewParser: function(content) {
	        var output = Opal.Asciidoctor.$render(content, Opal.hash2(['attributes'], {'attributes': ['notitle!']}));
	        return output
            },
            onShiftEnter: {keepDefault:false, openWith:'\n\n'},
            markupSet: [
	        {name:'Book title', key:'1', placeHolder:'Title here', openWith:'[[id_here]]\n= '},
	        {name:'Heading 2', key:'2', placeHolder:'Section name here',  openWith:'[[id_here]]\n== '},
	        {name:'Heading 3', key:'3', openWith:'[[id_here]]\n=== ', placeHolder:'Section name here' },
	        {name:'Heading 4', key:'4', openWith:'[[id_here]]\n==== ', placeHolder:'Section name here' },
	        {separator:'---------------' },
	        {name:'Bold', key:'B', openWith:'*', closeWith:'*'},
	        {name:'Italic', key:'I', openWith:'_', closeWith:'_'},
	        {name:'Monospace', key:'M', openWith:'+', closeWith:'+'},
	        {separator:'---------------' },
	        {name:'Unordered List', key:'U', openWith:'* ' },
	        {name:'Numbered List', key: 'N', openWith:'. '},
	        {separator:'---------------' },
	        {name:'Picture', key:'P', replaceWith:'image::[![Url:!:http://]!][[![Title]!]]'},
	        {name:'Link', key:'L', openWith:'[![Url:!:http://]!][', closeWith:']', placeHolder:'Link text here' },
	        {separator:'---------------'},
	        {name:'Code Block', key:'Q', openWith:'[source,php]\n----\n', closeWith:'\n----\n'},
	        {separator:'---------------'},
	        {name:'Preview', key:'P', call:'preview', className:"preview"}
            ]
        });
    });
})(jQuery);

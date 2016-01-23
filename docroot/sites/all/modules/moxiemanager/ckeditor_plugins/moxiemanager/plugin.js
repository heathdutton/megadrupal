
(function() {
	CKEDITOR.editorConfig = function(config) {
	  config.baseHref = Drupal.settings.basePath;
	  config.filebrowserBrowseUrl = '';
    config.filebrowserImageBrowseUrl = '';
    config.filebrowserFlashBrowseUrl = '';
    config.filebrowserUploadUrl = '';
    config.filebrowserImageUploadUrl = '';
    config.filebrowserFlashUploadUrl = '';
  }
	// Adds (additional) arguments to given url.
	//
	// @param {String}
	//            url The url.
	// @param {Object}
	//            params Additional parameters.
	function addQueryString( url, params ) {
		var queryString = [];

		if ( !params )
			return url;
		else {
			for ( var i in params )
				queryString.push( i + "=" + encodeURIComponent( params[ i ] ) );
		}

		return url + ( ( url.indexOf( "?" ) != -1 ) ? "&" : "?" ) + queryString.join( "&" );
	}

	// Make a string's first character uppercase.
	//
	// @param {String}
	//            str String.
	function ucFirst( str ) {
		str += '';
		var f = str.charAt( 0 ).toUpperCase();
		return f + str.substr( 1 );
	}

	// The onlick function assigned to the 'Browse Server' button. Opens the
	// file browser and updates target field when file is selected.
	//
	// @param {CKEDITOR.event}
	//            evt The event object.
	function browseServer( evt ) {
		var dialog = this.getDialog();
		var editor = dialog.getParentEditor();
		editor._.filebrowserSe = this;
		var CKEditorFuncNum = editor._.filebrowserFn;
		window.moxman.browse({
		  no_host: true,
		  document_base_url: Drupal.settings.basePath,
		  oninsert: function(args) {
    	  CKEDITOR.tools.callFunction(CKEditorFuncNum, args.files[0].url);
       }
    });
  }

	// The onlick function assigned to the 'Upload' button. Makes the final
	// decision whether form is really submitted and updates target field when
	// file is uploaded.
	//
	// @param {CKEDITOR.event}
	//            evt The event object.
	function uploadFile( evt ) {
		var dialog = this.getDialog();
		var editor = dialog.getParentEditor();

		editor._.moxiemanagerSe = this;

		// If user didn't select the file, stop the upload.
		if ( !dialog.getContentElement( this[ 'for' ][ 0 ], this[ 'for' ][ 1 ] ).getInputElement().$.value )
			return false;

		if ( !dialog.getContentElement( this[ 'for' ][ 0 ], this[ 'for' ][ 1 ] ).getAction() )
			return false;

		return true;
	}

	// Setups the file element.
	//
	// @param {CKEDITOR.ui.dialog.file}
	//            fileInput The file element used during file upload.
	// @param {Object}
	//            moxiemanager Object containing moxiemanager settings assigned to
	//            the fileButton associated with this file element.
	function setupFileElement( editor, fileInput, moxiemanager ) {
		var params = moxiemanager.params || {};
		params.CKEditor = editor.name;
		params.CKEditorFuncNum = editor._.moxiemanagerFn;
		if ( !params.langCode )
			params.langCode = editor.langCode;

		fileInput.action = addQueryString( moxiemanager.url, params );
		fileInput.moxiemanager = moxiemanager;
	}


	// Traverse through the content definition and attach filebrowser to
	// elements with 'filebrowser' attribute.
	//
	// @param String
	//            dialogName Dialog name.
	// @param {CKEDITOR.dialog.definitionObject}
	//            definition Dialog definition.
	// @param {Array}
	//            elements Array of {@link CKEDITOR.dialog.definition.content}
	//            objects.
	function attachFileBrowser( editor, dialogName, definition, elements ) {
		if ( !elements || !elements.length )
			return;

		var element, fileInput;

		for ( var i = elements.length; i--; ) {
			element = elements[ i ];

			if ( element.type == 'hbox' || element.type == 'vbox' || element.type == 'fieldset' )
				attachFileBrowser( editor, dialogName, definition, element.children );

			if ( !element.filebrowser )
				continue;

			if ( typeof element.filebrowser == 'string' ) {
				var fb = {
					action: ( element.type == 'fileButton' ) ? 'QuickUpload' : 'Browse',
					target: element.filebrowser
				};
				element.filebrowser = fb;
			}

			if ( element.filebrowser.action == 'Browse' ) {
				element.onClick = browseServer;
				// element.filebrowser.url = url;
				element.hidden = false;
			}
		}
	}

	// Updates the target element with the url of uploaded/selected file.
	//
	// @param {String}
	//            url The url of a file.
	function updateTargetElement( url, sourceElement ) {
		var dialog = sourceElement.getDialog();
		var targetElement = sourceElement.moxiemanager.target || null;

		// If there is a reference to targetElement, update it.
		if ( targetElement ) {
			var target = targetElement.split( ':' );
			var element = dialog.getContentElement( target[ 0 ], target[ 1 ] );
			if ( element ) {
				element.setValue( url );
				dialog.selectPage( target[ 0 ] );
			}
		}
	}

	// Returns true if moxiemanager is configured in one of the elements.
	//
	// @param {CKEDITOR.dialog.definitionObject}
	//            definition Dialog definition.
	// @param String
	//            tabId The tab id where element(s) can be found.
	// @param String
	//            elementId The element id (or ids, separated with a semicolon) to check.
	function isConfigured( definition, tabId, elementId ) {
		if ( elementId.indexOf( ";" ) !== -1 ) {
			var ids = elementId.split( ";" );
			for ( var i = 0; i < ids.length; i++ ) {
				if ( isConfigured( definition, tabId, ids[ i ] ) )
					return true;
			}
			return false;
		}

		var elementFileBrowser = definition.getContents( tabId ).get( elementId ).moxiemanager;
		return ( elementFileBrowser && elementFileBrowser.url );
	}

	function setUrl( fileUrl, data ) {
		var dialog = this._.moxiemanagerSe.getDialog(),
			targetInput = this._.moxiemanagerSe[ 'for' ],
			onSelect = this._.moxiemanagerSe.moxiemanager.onSelect;

		if ( targetInput )
			dialog.getContentElement( targetInput[ 0 ], targetInput[ 1 ] ).reset();

		if ( typeof data == 'function' && data.call( this._.moxiemanagerSe ) === false )
			return;

		if ( onSelect && onSelect.call( this._.moxiemanagerSe, fileUrl, data ) === false )
			return;

		// The "data" argument may be used to pass the error message to the editor.
		if ( typeof data == 'string' && data )
			alert( data );

		if ( fileUrl )
			updateTargetElement( fileUrl, this._.moxiemanagerSe );
	}

	CKEDITOR.plugins.add( 'moxiemanager', {
		requires: 'popup',
		init: function( editor, pluginPath ) {
			editor._.moxiemanagerFn = CKEDITOR.tools.addFunction( setUrl, editor );
			editor.on( 'destroy', function() {
				CKEDITOR.tools.removeFunction( this._.moxiemanagerFn );
			});
		}
	});

	CKEDITOR.on( 'dialogDefinition', function( evt ) {
		var definition = evt.data.definition,
			element;

		// Associate moxiemanager to elements with 'moxiemanager' attribute.
		for ( var i = 0; i < definition.contents.length; ++i ) {
			if ( ( element = definition.contents[ i ] ) ) {
				attachFileBrowser( evt.editor, evt.data.name, definition, element.elements );
				if ( element.hidden && element.moxiemanager ) {
					element.hidden = !isConfigured( definition, element[ 'id' ], element.moxiemanager );
				}
			}
		}
	});


})();


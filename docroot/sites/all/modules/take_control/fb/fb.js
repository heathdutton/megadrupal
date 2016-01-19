
Ext.ns('Drupal.take_Control_fb');
var fb = Drupal.take_Control_fb;

fb.settings = window.parent.Drupal.settings.take_control_fb;
Ext.BLANK_IMAGE_URL = fb.settings.extPath + '/resources/images/default/s.gif';
fb.rootFolder = fb.settings.rootFolder;
fb.curFolder = '';
fb.prevFolders = [];
fb.nextFolders = [];

fb.zipMimes = fb.settings.zipMimes;

Ext
		.onReady(function() {
			Ext.QuickTips.init();

			var fileStore = new Ext.data.JsonStore( {
				// store configs
				autoDestroy: true,
				// reader configs
				root: 'files',
				idProperty: 'id',
				fields: [ 'id', 'text', 'cls', 'modified', 'owner', 'perm', 'mime',
						'size' ],
				data: {
					files: []
				}
			});
			var fileSelModel = new Ext.grid.CheckboxSelectionModel( {
				checkOnly: true,
				listeners: {
					selectionChange: fb.filesSelectionChanged
				}
			});

			var p = new Ext.Panel(
					{
						title: 'File Browser',
						renderTo: 'take_control_fb_container',
						autoWidth: true,
						height: 500,
						layout: 'hbox',
						layoutConfig: {
							align: 'stretch'
						},
						tbar: {
							xtype: 'toolbar',
							items: [ {
								xtype: 'button',
								text: 'New Folder',
								iconCls: 'icon-newfolder',
								ref: '../btnNewFolder',
								handler: fb.btnNewFolderClicked
							}, {
								xtype: 'button',
								text: 'New File',
								iconCls: 'icon-newfile',
								ref: '../btnNewFile',
								handler: fb.btnNewFileClicked
							}, {
								xtype: 'button',
								text: 'Copy',
								iconCls: 'icon-copy',
								ref: '../btnCopy',
								disabled: true,
								handler: fb.btnCopyFileClicked
							}, {
								xtype: 'button',
								text: 'Move',
								iconCls: 'icon-move',
								ref: '../btnMove',
								disabled: true,
								handler: fb.btnMoveFileClicked
							}, {
								xtype: 'button',
								text: 'Upload',
								iconCls: 'icon-upload',
								ref: '../btnUpload',
								handler: fb.btnUploadClicked
							}, {
								xtype: 'button',
								text: 'Download',
								iconCls: 'icon-download',
								ref: '../btnDownload',
								disabled: true,
								handler: fb.btnDownloadClicked
							}, {
								xtype: 'button',
								text: 'Delete',
								iconCls: 'icon-delete',
								ref: '../btnDelete',
								disabled: true,
								handler: fb.btnDeleteClicked
							}, {
								xtype: 'tbseparator'
							}, {
								xtype: 'button',
								text: 'Rename',
								iconCls: 'icon-rename',
								ref: '../btnRename',
								disabled: true,
								handler: fb.btnRenameClicked
							}, {
								xtype: 'button',
								text: 'Edit',
								iconCls: 'icon-edit',
								ref: '../btnEdit',
								disabled: true,
								hidden: true
							}, {
								xtype: 'button',
								text: 'Code Editor',
								iconCls: 'icon-editcode',
								ref: '../btnEditCode',
								disabled: true,
								hidden: true
							}, {
								xtype: 'button',
								text: 'HTML Editor',
								iconCls: 'icon-edithtml',
								ref: '../btnEditHtml',
								disabled: true,
								hidden: true
							}, {
								xtype: 'tbseparator'
							}, {
								xtype: 'button',
								text: 'Extract',
								iconCls: 'icon-extract',
								ref: '../btnExtract',
								disabled: true,
								handler: fb.btnExtractClicked
							}, {
								xtype: 'button',
								text: 'Compress',
								iconCls: 'icon-compress',
								ref: '../btnCompress',
								disabled: true,
								handler: fb.btnCompressClicked
							} ]
						},
						bbar: {
							xtype: 'toolbar',
							items: [ {
								xtype: 'label',
								text: 'Current User Id: ' + fb.settings.curUserId
							} ]
						},
						items: [
								{
									xtype: 'treepanel',
									layout: 'fit',
									flex: 3,
									title: 'Folder Explorer',
									ref: 'folderPanel',
									dataUrl: fb.settings.dataUrl,
									autoScroll: true,
									useArrows: true,
									animate: true,
									containerScroll: true,
									requestMethod: 'POST',
									trackMouseOver: true,
									listeners: {
										'click': fb.foldersNodeClicked
									},
									rootVisible: true,
									root: {
										nodeType: 'async',
										id: fb.rootFolder,
										text: fb.rootFolder,
										draggable: false,
										expanded: true
									},
									loader: {
										url: fb.settings.dataUrl,
										baseParams: fb.getRequestParams('list-subdirs', {}),
										listeners: {
											loadException: function(loader, node, response) {
												Ext.Msg.alert('Action Failure', response.responseText);
											}
										// 'beforeload': function(loader, node, callback) {
										// var tree = node.ownerTree;
										// if (!tree.loadMask) {
										// tree.loadMask = new Ext.LoadMask(tree.getEl().dom, {
										// msg: 'Loading...'
										// });
										// }
										// tree.loadMask.show();
										// },
										// 'load': function(loader, node, response) {
										// node.ownerTree.loadMask.hide();
										// }
										}
									},
									tbar: {
										xtype: 'toolbar',
										items: [
												{
													xtype: 'tbtext',
													'text': 'Current: '
												},
												{
													xtype: 'textfield',
													width: 200,
													ref: '../txtCurrentDir'
												},
												{
													xtype: 'button',
													text: 'Go',
													listeners: {
														'click': function(el) {
															fb.nextFolders.splice(0, fb.nextFolders.length);
															fb.loadFiles(fb.folders.txtCurrentDir.getValue(),
																	true);
														}
													}
												} ]
									},
									tools: [
											{
												id: 'up',
												qtip: 'Collpase All',
												handler: function(event, toolEl, panel) {
													fb.folders.collapseAll();
												}
											},
											{
												id: 'down',
												qtip: 'Expand All',
												handler: function(event, toolEl, panel) {
													Ext.Msg
															.confirm(
																	'Action confirmation',
																	'Are you sure you want to expand all folders?<br /> This might take a lot of time depending upon the number of folders.',
																	function(btn) {
																		if (btn == 'yes') {
																			// This will also expand nodes as they
																			// arrive. So, better copy nodeHash, and
																			// then expand manually.
																			// fb.folders.expandAll();
																			var nodes = [];
																			for (key in fb.folders.nodeHash) {
																				nodes.push(fb.folders.nodeHash[key]);
																			}
																			for (i = 0; i < nodes.length; i++) {
																				nodes[i].expand();
																			}
																		}
																	});
												}
											},
											{
												id: 'gear',
												qtip: 'Recursive Expand All',
												handler: function(event, toolEl, panel) {
													Ext.Msg
															.confirm(
																	'Action confirmation',
																	'Are you sure you want to recursively expand all folders?<br /> This might take <b>excessively large time</b> depending upon the number of folders.',
																	function(btn) {
																		if (btn == 'yes') {
																			fb.folders.expandAll();
																		}
																	});
												}
											}, {
												id: 'refresh',
												qtip: 'Refresh',
												handler: function(event, toolEl, panel) {
													var root = fb.folders.getRootNode();
													fb.folders.loader.load(root, function(node) {
														node.expand();
													});
												}
											} ]
								},
								{
									xtype: 'grid',
									flex: 7,
									title: 'File Explorer',
									ref: 'filePanel',
									store: fileStore,
									autoExpandColumn: 'name',
									stripeRows: true,
									listeners: {
										'cellclick': fb.filesCellClick,
										'rowdblclick': fb.filesRowDblClick
									},
									sm: fileSelModel,
									cm: new Ext.grid.ColumnModel(
											[
													new Ext.grid.RowNumberer(),
													fileSelModel,
													{
														header: '',
														sortable: false,
														dataIndex: 'cls',
														width: 30,
														renderer: function(value, metaData, record,
																rowIndex, colIndex, store) {
															var img = '<img src="{0}" class="{1}" title="{2}" width="16" height="16" />'
															img = String.format(img, Ext.BLANK_IMAGE_URL,
																	value == 'folder' ? 'icon-folder'
																			: 'icon-file', value);
															return (img);
														}
													}, {
														header: 'Name',
														sortable: true,
														dataIndex: 'text',
														id: 'name'
													}, {
														header: 'Size',
														sortable: true,
														dataIndex: 'size',
														width: 50
													}, {
														header: 'Type',
														sortable: true,
														dataIndex: 'mime',
														width: 150
													}, {
														header: 'Owner',
														sortable: true,
														dataIndex: 'owner',
														width: 30
													}, {
														header: 'Perms',
														sortable: true,
														dataIndex: 'perm',
														width: 50
													}, {
														header: 'Last Modified',
														sortable: true,
														dataIndex: 'modified',
														width: 150
													} ]),
									tbar: {
										xtype: 'toolbar',
										items: [ {
											xtype: 'button',
											text: 'Home',
											iconCls: 'icon-home',
											ref: '../btnHome',
											disabled: true,
											listeners: {
												'click': fb.btnHomeClicked
											}
										}, {
											xtype: 'button',
											text: 'Up One Level',
											iconCls: 'icon-uplevel',
											ref: '../btnFolderUp',
											disabled: true,
											listeners: {
												'click': fb.btnFolderUpClicked
											}
										}, {
											xtype: 'button',
											text: 'Back',
											iconCls: 'icon-back',
											ref: '../btnFolderPrev',
											disabled: true,
											listeners: {
												'click': fb.btnFolderPrevClicked
											}
										}, {
											xtype: 'button',
											text: 'Forward',
											iconCls: 'icon-forward',
											ref: '../btnFolderNext',
											disabled: true,
											listeners: {
												'click': fb.btnFolderNextClicked
											}
										}, {
											xtype: 'button',
											text: 'Reload',
											iconCls: 'icon-refresh',
											ref: '../btnFilesRefresh',
											listeners: {
												'click': fb.btnFilesRefreshClicked
											}
										}, {
											xtype: 'tbseparator'
										}, {
											xtype: 'button',
											text: 'Change Permissions',
											iconCls: 'icon-permission',
											ref: '../btnPerm',
											disabled: true,
											handler: fb.btnPermClicked
										}, {
											xtype: 'button',
											text: 'View',
											iconCls: 'icon-view',
											ref: '../btnView',
											disabled: true,
											handler: fb.btnViewClicked
										} ]
									}
								} ]
					});

			fb.parent = p;
			fb.folders = p.folderPanel;
			fb.files = p.filePanel;

			new Ext.tree.TreeSorter(fb.folders, {
				folderSort: true
			});

			// Load root directory nodes.
			fb.loadFiles(fb.rootFolder, false);

			fb.createFileViewWindow();
			fb.createFileUploadWindow();

		});

// /////////////////////////////////////////////////////////////////////////////
// Helper methods.

fb.getRequestParams = function(op, params) {
	params.op = op;
	params.validationString = fb.settings.validationString;
	params.validationToken = fb.settings.validationToken;
	// params.loginTime = fb.settings.loginTime;

	return (params);
}

fb.makeAjaxCall = function(op, params, callback) {
	fb.files.loadMask.show();

	Ext.Ajax.request( {
		url: fb.settings.dataUrl,
		params: fb.getRequestParams(op, params),
		method: 'POST',
		success: function(response, options) {
			fb.files.loadMask.hide();

			callback(response.responseText);
		}
	});
}

fb.createUrl = function(params) {
	var str = '';
	for (key in params) {
		str = str + key + '=' + encodeURIComponent(params[key]) + '&';
	}
	str = fb.settings.dataUrl + '?' + str.substring(0, str.length - 1);

	return (str);
}

fb.getParent = function(path) {
	var index = path.lastIndexOf('/');
	if (index == -1) {
		index = path.lastIndexOf('\\');
	}
	var parent = path.substring(0, index);

	return (parent);
}

fb.getSelectedNodes = function() {
	var selections = fb.files.selModel.getSelections();
	var ids = [];
	for (i = 0; i < selections.length; i++) {
		ids.push(selections[i].data.id);
	}
	return (ids);
}

fb.formatNodes = function(nodes) {
	var html = '<div style="border: 1px dotted; max-height: 75px; overflow: scroll">';

	for (i = 0; i < nodes.length - 1; i++) {
		html += nodes[i] + '<br />';
	}
	html += nodes[i] + '</div>';

	return (html);
}

fb.reloadFolder = function(dir) {
	var node = fb.folders.getNodeById(dir);
	var expanded = node.isExpanded();
	if (!Ext.isEmpty(node)) {
		fb.folders.loader.load(node, function(node) {
			if (expanded) {
				node.expand();
			}
		});
	}
}

fb.loadFiles = function(dir, addToPrev) {
	if (dir.indexOf(fb.rootFolder) != 0) {
		if (dir.charAt(0) != '/')
			dir = '/' + dir;
		dir = fb.rootFolder + dir;
	}

	if (!fb.files.loadMask) {
		fb.files.loadMask = new Ext.LoadMask(fb.files.getEl().dom, {
			msg: 'Loading...'
		});
	}
	var gridEl = fb.files.getGridEl();
	if (gridEl.isMasked()) {
		gridEl.unmask();
	}
	fb.files.loadMask.show();

	Ext.Ajax.request( {
		url: fb.settings.dataUrl,
		params: fb.getRequestParams('list-files', {
			node: dir
		}),
		method: 'POST',
		success: function(response, options) {
			fb.files.loadMask.hide();
			fb.files.selModel.clearSelections();

			var json = response.responseText;
			if (json.charAt(0) != '[' && json.charAt(0) != '{') {
				Ext.Msg.alert('Action Failure', json);
				return;
			}

			var files = Ext.decode(json);
			fb.files.store.loadData( {
				files: files
			}, false);

			if (files.length == 0) {
				gridEl.mask('The folder is empty', 'x-mask');
			}

			if (addToPrev && !Ext.isEmpty(fb.curFolder)) {
				fb.prevFolders.push(fb.curFolder);
			}
			fb.curFolder = dir;
			fb.files.btnFolderPrev.setDisabled(fb.prevFolders.length == 0);
			fb.files.btnFolderNext.setDisabled(fb.nextFolders.length == 0);
			fb.files.btnHome.setDisabled(fb.rootFolder == fb.curFolder);
			fb.files.btnFolderUp.setDisabled(fb.rootFolder == fb.curFolder);
			fb.folders.txtCurrentDir.setValue(dir);
		}
	});
}

// /////////////////////////////////////////////////////////////////////////////
// File Load button click handlers.

fb.foldersNodeClicked = function(node, e) {
	node.expand();
	fb.nextFolders.splice(0, fb.nextFolders.length);
	fb.loadFiles(node.id, true);
}

fb.filesRowDblClick = function(grd, rowIndex, e) {
	var rec = grd.store.data.items[rowIndex];
	if (rec.data.cls == 'folder') {
		fb.loadFiles(rec.data.id, true);
	}
}

fb.btnHomeClicked = function(el) {
	fb.nextFolders.splice(0, fb.nextFolders.length);
	var dir = fb.rootFolder;
	fb.loadFiles(dir, true);
}

fb.btnFolderUpClicked = function(el) {
	var index = fb.curFolder.lastIndexOf('/');
	if (index == -1)
		index = fb.curFolder.lastIndexOf('\\');
	if (index == -1) {
		Ext.Msg.alert('Action Failure', 'Invalid Root directory');
		return;
	}
	var dir = fb.curFolder.substring(0, index);
	fb.loadFiles(dir, true);
}

fb.btnFolderPrevClicked = function(el) {
	fb.nextFolders.push(fb.curFolder);
	var dir = fb.prevFolders.pop();
	fb.loadFiles(dir, false);
}

fb.btnFolderNextClicked = function(el) {
	var dir = fb.nextFolders.pop();
	fb.loadFiles(dir, true);
}

fb.btnFilesRefreshClicked = function(el) {
	fb.loadFiles(fb.curFolder, false);
}

// /////////////////////////////////////////////////////////////////////////////
// Button enable/disable handlers.

fb.filesCellClick = function(grd, rowIndex, colIndex, e) {
	if (colIndex != 1) {
		if (grd.selModel.isSelected(rowIndex)) {
			grd.selModel.deselectRow(rowIndex);
		} else {
			grd.selModel.selectRow(rowIndex, true);
		}
	}
}

fb.filesSelectionChanged = function(sm) {
	var noSel = !sm.hasSelection();
	var single = (sm.getCount() == 1);
	var isFile = false;
	if (single) {
		var rec = sm.getSelected();
		var isZip;
		Ext.each(fb.zipMimes, function(item) {
			if (item == rec.data.mime) {
				isZip = true;
				return (false);
			}
		});
		isFile = (rec.data.cls == 'file');
	}

	fb.parent.btnDownload.setDisabled(!single || !isFile);
	fb.parent.btnCopy.setDisabled(noSel);
	fb.parent.btnMove.setDisabled(noSel);
	fb.parent.btnDelete.setDisabled(noSel);

	fb.parent.btnRename.setDisabled(!single);
	fb.parent.btnEdit.setDisabled(!single || !isFile);
	fb.parent.btnEditCode.setDisabled(!single || !isFile);
	fb.parent.btnEditHtml.setDisabled(!single || !isFile);

	fb.parent.btnCompress.setDisabled(noSel);
	fb.parent.btnExtract.setDisabled(!(single && isZip));

	fb.files.btnPerm.setDisabled(noSel);
	fb.files.btnView.setDisabled(!single || !isFile);
}

// /////////////////////////////////////////////////////////////////////////////
// Toolbar button logic.

fb.btnNewFolderClicked = function(el) {
	var parent = fb.curFolder;
	Ext.Msg
			.prompt(
					'Create New folder',
					'Enter folder name. Folder would be created in <br /><b>' + fb.curFolder + '</b>',
					function(btn, value) {
						if (btn != 'ok' || Ext.isEmpty(value))
							return;
						fb.makeAjaxCall('create-folder', {
							parent: parent,
							name: value
						}, function(response) {
							if (!Ext.isEmpty(response)) {
								Ext.Msg.alert('Action Failure', response);
								return;
							}

							fb.loadFiles(parent, false);
							fb.reloadFolder(parent);
						});
					});
}

fb.btnNewFileClicked = function(el) {
	var parent = fb.curFolder;
	Ext.Msg
			.prompt(
					'Create New file',
					'Enter file name. File would be created in <br /><b>' + fb.curFolder + '</b>',
					function(btn, value) {
						if (btn != 'ok' || Ext.isEmpty(value))
							return;
						fb.makeAjaxCall('create-file', {
							parent: parent,
							name: value
						}, function(response) {
							if (!Ext.isEmpty(response)) {
								Ext.Msg.alert('Action Failure', response);
								return;
							}

							fb.loadFiles(parent, false);
						});
					});
}

fb.btnCopyFileClicked = function(el) {
	var ids = fb.getSelectedNodes();

	Ext.Msg.prompt('Copy file(s)/folder(s)',
			'Type the path you wish to copy to and press "Ok".<br /><br />' + fb
					.formatNodes(ids), function(btn, value) {
				if (btn != 'ok' || Ext.isEmpty(value))
					return;

				fb.makeAjaxCall('copy', {
					nodes: Ext.encode(ids),
					path: value
				}, function(response) {
					if (!Ext.isEmpty(response)) {
						Ext.Msg.alert('Action Failure', response);
						return;
					}

					fb.files.selModel.clearSelections();
					if (!Ext.isEmpty(fb.folders.getNodeById(value))) {
						fb.reloadFolder(value);
					}
				});
			}, this, false, fb.curFolder);
}

fb.btnMoveFileClicked = function(el) {
	var ids = fb.getSelectedNodes();

	Ext.Msg.prompt('Move file(s)/folder(s)',
			'Type the path you wish to move to and press "Ok".<br /><br />' + fb
					.formatNodes(ids), function(btn, value) {
				if (btn != 'ok' || Ext.isEmpty(value))
					return;

				fb.makeAjaxCall('move', {
					nodes: Ext.encode(ids),
					path: value
				}, function(response) {
					if (!Ext.isEmpty(response)) {
						Ext.Msg.alert('Action Failure', response);
						return;
					}

					fb.files.selModel.clearSelections();
					fb.loadFiles(fb.curFolder, false);
					if (!Ext.isEmpty(fb.folders.getNodeById(fb.curFolder))) {
						fb.reloadFolder(fb.curFolder);
					}
				});
			}, this, false, fb.curFolder);
}

fb.btnDownloadClicked = function(el) {
	try {
		Ext.destroy(Ext.get('take_control_fb_container_download_frame'));
	} catch (e) {
	}

	var rec = fb.files.selModel.getSelected();
	var params = fb.getRequestParams('download-file', {
		file: rec.data.id
	});

	var url = fb.createUrl(params);

	Ext.DomHelper.append(document.body, {
		tag: 'iframe',
		id: 'take_control_fb_container_download_frame',
		frameBorder: 0,
		width: 0,
		height: 0,
		css: 'display:none;visibility:hidden;height:0px;',
		src: url
	});
}

fb.btnDeleteClicked = function(el) {
	var ids = fb.getSelectedNodes();

	Ext.Msg
			.confirm(
					'Delete file(s)/folder(s)',
					'Are you sure you want to delete the selected file(s)/folder(s)?<br />Please note that this action cannot be undone.<br /><br />' + fb
							.formatNodes(ids), function(btn) {
						if (btn != 'yes')
							return;

						fb.makeAjaxCall('delete', {
							nodes: Ext.encode(ids)
						}, function(response) {
							if (!Ext.isEmpty(response)) {
								Ext.Msg.alert('Action Failure', response);
								return;
							}

							fb.files.selModel.clearSelections();
							fb.reloadFolder(fb.curFolder);
							fb.loadFiles(fb.curFolder, false);
						});
					});
}

fb.btnUploadClicked = function(el) {
	fb.uploadWindow.lblTo.setText(fb.curFolder);
	fb.uploadWindow.show();
}

fb.btnRenameClicked = function(el) {
	var rec = fb.files.selModel.getSelected();

	Ext.Msg.prompt('Rename file/folder',
			'Enter new name for:<br /><b>' + rec.data.id + '</b>', function(btn,
					value) {
				if (btn != 'ok' || Ext.isEmpty(value))
					return;

				fb.makeAjaxCall('rename', {
					path: rec.data.id,
					name: value
				}, function(response) {
					if (!Ext.isEmpty(response)) {
						Ext.Msg.alert('Action Failure', response);
						return;
					}

					fb.loadFiles(fb.curFolder, false);
					if (rec.data.cls == 'folder') {
						fb.reloadFolder(fb.getParent(rec.data.id));
					}
				});
			});
}

fb.btnExtractClicked = function(el) {
	var rec = fb.files.selModel.getSelected();

	Ext.Msg
			.prompt(
					'Extract archive',
					'Enter the path you wish to extract:<br /><b>' + rec.data.id + '</b><br />to (if you enter a directory that does not exist it will be created, and the archive extracted in the new directory, files with same names for an existing directory would be overwritten) and click "Ok".',
					function(btn, value) {
						if (btn != 'ok' || Ext.isEmpty(value))
							return;

						fb.makeAjaxCall('extract-archive', {
							path: value,
							file: rec.data.id
						}, function(response) {
							if (!Ext.isEmpty(response)) {
								Ext.Msg.alert('Action Failure', response);
								return;
							}

							fb.files.selModel.clearSelections();
							if (value == fb.curFolder) {
								fb.loadFiles(fb.curFolder, false);
								fb.reloadFolder(fb.getParent(rec.data.id));
							}
						});
					}, this, false, fb.curFolder);
}

fb.btnCompressClicked = function(el) {
	var ids = fb.getSelectedNodes();

	Ext.Msg
			.prompt(
					'Create Zip archive',
					'Enter the name of the compressed archive to create (.zip would be appended if the name does not end with it and an existing file with the same name would be overwritten) and click "Ok".<br /><br />' + fb
							.formatNodes(ids), function(btn, value) {
						if (btn != 'ok' || Ext.isEmpty(value))
							return;

						fb.makeAjaxCall('create-archive', {
							nodes: Ext.encode(ids),
							file: value,
							curDir: fb.curFolder
						}, function(response) {
							if (!Ext.isEmpty(response)) {
								Ext.Msg.alert('Action Failure', response);
								return;
							}

							fb.files.selModel.clearSelections();
							fb.loadFiles(fb.curFolder, false);
						});
					}, this, false, fb.curFolder + fb.settings.dirSeparator
							+ fb.files.selModel.getSelections()[0].data.text + '.zip');
}

fb.btnPermClicked = function(el) {
	var ids = fb.getSelectedNodes();

	Ext.Msg.prompt('Change Permissions', 'Enter new Permission:', function(btn,
			value) {
		if (btn != 'ok' || Ext.isEmpty(value))
			return;

		fb.makeAjaxCall('change-perm', {
			nodes: Ext.encode(ids),
			perm: value
		}, function(response) {
			if (!Ext.isEmpty(response)) {
				Ext.Msg.alert('Action Failure', response);
				return;
			}

			fb.loadFiles(fb.curFolder, false);
		});
	});
}

fb.btnViewClicked = function(el) {
	var rec = fb.files.selModel.getSelected();

	fb.makeAjaxCall('view-file', {
		file: rec.data.id
	}, function(response) {
		fb.viewWindow.txtView.setValue(response);
		fb.viewWindow.show();
		fb.viewWindow.center();
		fb.viewWindow.setTitle(rec.data.id);
	});
}

// /////////////////////////////////////////////////////////////////////////////
// Window Creation.

fb.createFileViewWindow = function() {
	fb.viewWindow = new Ext.Window( {
		width: 500,
		height: 400,
		layout: 'fit',
		closeAction: 'hide',
		modal: true,
		maximizable: true,
		title: 'File Viewer',
		items: [ {
			xtype: 'textarea',
			ref: 'txtView'
		} ],
		bbar: {
			xtype: 'toolbar',
			buttonAlign: 'right',
			items: [ {
				xtype: 'button',
				text: 'Close',
				handler: function(el) {
					fb.viewWindow.hide();
				}
			} ]
		}
	});
}

fb.createFileUploadWindow = function() {
	fb.uploadCount = 0;
	var getField = function() {
		var f = new Ext.form.TextField( {
			fieldLabel: 'File',
			name: 'uploaded-file' + fb.uploadCount++,
			buttonText: '',
			inputType: 'file',
			buttonCfg: {
				iconCls: 'icon-upload'
			}
		});

		return (f);
	}

	var resetUpload = function() {
		fb.uploadWindow.pnlUpload.removeAll(true);
		fb.uploadWindow.pnlUpload.add(getField());
		fb.uploadWindow.pnlUpload.doLayout();
	}

	fb.uploadWindow = new Ext.Window(
			{
				width: 400,
				height: 275,
				layout: 'fit',
				closeAction: 'hide',
				modal: true,
				title: 'Upload File(s)',
				tbar: {
					xtype: 'toolbar',
					items: [ {
						xtype: 'label',
						ref: '../lblTo'
					} ]
				},
				items: [ {
					xtype: 'form',
					ref: 'pnlUpload',
					fileUpload: true,
					frame: true,
					autoScroll: true,
					bodyStyle: 'padding: 10px 10px 0 10px;',
					labelWidth: 50,
					defaults: {
						anchor: '90%',
						allowBlank: false,
						msgTarget: 'side'
					},

					items: [ getField() ],
					bbar: {
						xtype: 'toolbar',
						items: [ {
							xtype: 'label',
							html: '<b>Warning</b>: Existing files with same name would be overwritten.'
						} ]
					}
				} ],
				bbar: {
					xtype: 'toolbar',
					buttonAlign: 'right',
					items: [
							{
								xtype: 'button',
								text: 'Add Another',
								handler: function(el) {
									fb.uploadWindow.pnlUpload.add(getField());
									fb.uploadWindow.pnlUpload.doLayout();
								}
							},
							{
								xtype: 'button',
								text: 'Upload files',
								handler: function(el) {
									var params = fb.getRequestParams('upload-file', {
										parent: fb.curFolder
									});
									var url = fb.createUrl(params);
									fb.uploadWindow.pnlUpload
											.getForm()
											.submit(
													{
														url: url,
														waitMsg: 'Uploading file(s)...',
														success: function(fp, o) {
															var suc = eval(o.result.succeded);
															var fai = eval(o.result.failed);
															if (suc.length > 0) {
																var s = 'The following files were uploaded successfully:<br />';
																for (i = 0; i < suc.length; i++) {
																	s = s + suc[i] + '<br />';
																}
															}
															if (fai.length > 0) {
																var s = 'The upload for following files failed:<br />';
																for (i = 0; i < fai.length; i++) {
																	s = s + fai[i] + '<br />';
																}
															}

															Ext.Msg.alert('Action Result', s);
															fb.loadFiles(fb.curFolder, false);
															fb.uploadWindow.hide();
															resetUpload();
														},
														failure: function(form, action) {
															switch (action.failureType) {
																case Ext.form.Action.CLIENT_INVALID:
																	Ext.Msg
																			.alert('Failure',
																					'Form fields may not be submitted with invalid values');
																	break;
																case Ext.form.Action.CONNECT_FAILURE:
																	Ext.Msg.alert('Failure',
																			'Ajax communication failed');
																	break;
																case Ext.form.Action.SERVER_INVALID:
																	Ext.Msg.alert('Failure', action.result.msg);
															}
														}
													});

								}
							}, {
								xtype: 'button',
								text: 'Reset',
								handler: function(el) {
									resetUpload();
								}
							}, {
								xtype: 'button',
								text: 'Close',
								handler: function(el) {
									fb.uploadWindow.hide();
								}
							} ]
				}
			});
}

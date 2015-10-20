/*
	This file is a part of myTinyTodo.
	(C) Copyright 2010 Max Pozdeev <maxpozdeev@gmail.com>
	Licensed under the GNU GPL v2+ license. See file COPYRIGHT for details.
*/

// AJAX myTinyTodo Storage

(function($){

var mtt;
var base_path = base_url ? base_url : '/';
if (!clean_url) {
	base_path += '?q=';
}
var add_args = clean_url ? '?' : '&';

function mytinytodoStorageAjax(amtt) 
{
	this.mtt = mtt = amtt;
}

window.mytinytodoStorageAjax = mytinytodoStorageAjax;

mytinytodoStorageAjax.prototype = 
{
	/* required method */
	request:function(action, params, callback)
	{
		if (!this[action]) 
			throw "Unknown storage action: " + action;

		this[action](params, function(json){
			if (json.denied)
				mtt.errorDenied();
			if (callback)
				callback.call(mtt, json)
		});
	},


	loadLists: function(params, callback)
	{
		$.getJSON(base_path + 'mytinytodo/ajax' + add_args + 'loadLists'+'&rnd='+Math.random() + '&fid=' + field_id, callback);
	},


	loadTasks: function(params, callback)
	{
		var q = '';
		if(params.search && params.search != '') q += '&s='+encodeURIComponent(params.search);
		if(params.tag && params.tag != '') q += '&t='+encodeURIComponent(params.tag);
		if(params.setCompl && params.setCompl != 0) q += '&setCompl=1';
		q += '&rnd='+Math.random();

		$.getJSON(base_path + 'mytinytodo/ajax' + add_args + 'loadTasks&list='+params.list+'&compl='+params.compl+'&sort='+params.sort+q + '&fid=' + field_id, callback);
	},


	newTask: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'newTask',
			{ list:params.list, title: params.title, tag:params.tag, fid: field_id, csrf: csrf_token }, callback, 'json');
	},
	

	fullNewTask: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'fullNewTask',
			{ list:params.list, title:params.title, note:params.note, prio:params.prio, tags:params.tags, duedate:params.duedate, fid: field_id, csrf: csrf_token },
			callback, 'json');
	},


	editTask: function(params, callback)
	{
            $.post(base_path + 'mytinytodo/ajax' + add_args + 'editTask='+params.id,
                { id:params.id, title:params.title, note:params.note, prio:params.prio, tags:params.tags, duedate:params.duedate, fid: field_id, csrf: csrf_token },
                callback, 'json');
	},


	editNote: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'editNote='+params.id, { id:params.id, note: params.note, fid: field_id, csrf: csrf_token }, callback, 'json');
	},


	completeTask: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'completeTask='+params.id, { id:params.id, compl:params.compl, fid: field_id, csrf: csrf_token }, callback, 'json');
	},


	deleteTask: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'deleteTask='+params.id, { id:params.id, fid: field_id, csrf: csrf_token }, callback, 'json');
	},


	setPrio: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'setPrio='+params.id, { id:params.id, prio: params.prio, rnd: Math.random(), fid: field_id, csrf: csrf_token }, callback);
	},

	
	setSort: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'setSort', { list:params.list, sort:params.sort, fid: field_id, csrf: csrf_token }, callback, 'json');
	},

	changeOrder: function(params, callback)
	{
		var order = '';
		for(var i in params.order) {
			order += params.order[i].id +'='+ params.order[i].diff + '&';
		}
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'changeOrder', { order:order, fid: field_id, csrf: csrf_token }, callback, 'json');
	},

	tagCloud: function(params, callback)
	{
		$.getJSON(base_path + 'mytinytodo/ajax' + add_args + 'tagCloud&list='+params.list+'&rnd='+Math.random() + '&fid=' + field_id, callback);
	},

	moveTask: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'moveTask', { id:params.id, from:params.from, to:params.to, fid: field_id, csrf: csrf_token }, callback, 'json');
	},

	parseTaskStr: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'parseTaskStr', { list:params.list, title:params.title, tag:params.tag, fid: field_id, csrf: csrf_token }, callback, 'json');
	},
	

	// Lists
	addList: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'addList', { name:params.name, fid: field_id, csrf: csrf_token }, callback, 'json'); 

	},

	renameList:  function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'renameList', { list:params.list, name:params.name, fid: field_id, csrf: csrf_token }, callback, 'json');
	},

	deleteList: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'deleteList', { list:params.list, fid: field_id, csrf: csrf_token }, callback, 'json');
	},

	publishList: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'publishList', { list:params.list, publish:params.publish, fid: field_id, csrf: csrf_token },  callback, 'json');
	},
	
	setShowNotesInList: function(params, callback)
	{
	    $.post(base_path + 'mytinytodo/ajax' + add_args + 'setShowNotesInList', { list:params.list, shownotes:params.shownotes, fid: field_id, csrf: csrf_token },  callback, 'json');
	},
	
	setHideList: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'setHideList', { list:params.list, hide:params.hide, fid: field_id, csrf: csrf_token }, callback, 'json');
	},

	changeListOrder: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'changeListOrder', { order:params.order, fid: field_id, csrf: csrf_token }, callback, 'json');
	},

	clearCompletedInList: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax' + add_args + 'clearCompletedInList', { list:params.list, fid: field_id, csrf: csrf_token }, callback, 'json');
	}

};

})(jQuery);

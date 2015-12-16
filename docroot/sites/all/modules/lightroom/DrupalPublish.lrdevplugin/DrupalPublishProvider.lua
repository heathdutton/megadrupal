-- Lightroom SDK
local LrView = import 'LrView'
local LrPathUtils = import 'LrPathUtils'
local LrDialogs = import 'LrDialogs'
local LrErrors = import 'LrErrors'
local LrHttp = import 'LrHttp'

local LrLogger = import 'LrLogger'

JSON = (loadfile(LrPathUtils.child(_PLUGIN.path, "JSON.lua")))() -- one-time load of the routines

local logger = LrLogger( 'console' )
logger:enable( "print" ) -- or "logfile"

local headers = {
  { field = 'Content-Type', value = 'application/json' },
  { field = 'Accept', value = 'application/json' },
}

local DrupalPublish = {
  titleForPublishedCollection = 'Node',
  titleForPublishedCollection_standalone = 'Node',
  titleForPublishedSmartCollection = 'Smart Node',
  titleForPublishedSmartCollection_standalone = 'Smart Node',
  supportsIncrementalPublish = 'only',
  supportsCustomSortOrder = true,
  exportPresetFields = {
    { key = 'url', default = '' },
    { key = 'username', default = '' },
    { key = 'password', default = '' },
  },
  showSections = {
    'imageSettings'
  },
  allowFileFormats = { 'JPEG' }
  --  small_icon = 'icon.small.png',
}

DrupalPublish.getUserToken = function ( props )

  local body, response = LrHttp.get( props.url .. 'services/session/token', headers)

  if (response.status == 200) then
    headers = {
      { field = 'Content-Type', value = 'application/json' },
      { field = 'Accept', value = 'application/json' },
      { field = 'X-CSRF-Token', value = body },
    }
    return body
  end

end

DrupalPublish.userLogin = function ( props )

  -- The user login process is this:
  --   Get a CSRF token
  --   Use that token to get the current user
  --   If the user is anonymous, login
  --     @todo how to handle if anonymous users aren't allowed? is that even possible?
  --   If login successful, get a new CSRF token
  --   Return the user and the CSRF token

  -- see if we're logged in
  -- we need a user token to post, even if we're anonymous (i think)
  local userToken = DrupalPublish.getUserToken( props )
  local body, response = LrHttp.post( props.url .. 'lightroom/system/connect', '', headers)
  local data = JSON.decode(body)

  if not (response.status == 200) then
    logger:trace('System connect error')
    LrErrors.throwUserError( 'Unable to connect.' )
  end
  if not (data.user) then
    logger:trace('System connect error')
    LrErrors.throwUserError( 'Unable to get user.' )
  end

  local user = data.user
  if user.uid == 0 then

    -- User login
    local data = {
      username = props.username,
      password = props.password
    }
    local body, response = LrHttp.post( props.url .. 'lightroom/user/login', JSON.encode(data), {
      { field = 'Content-Type', value = 'application/json' },
      { field = 'Accept', value = 'application/json' },
    })
    local data = JSON.decode(body)

    if not (response.status == 200) then
      logger:trace('User login error')
      if data then
        local message = table.concat(data, '\n')
        LrErrors.throwUserError( message )
      else
        LrErrors.throwUserError( 'User login failed. Please check the URL, user name, and password in the Publish Settings dialog, and confirm that Services are properly configured on your web site.' )
      end
    end

    if not (data.user) then
      LrErrors.throwUserError( 'Unable to get user.' )
    end

    -- Set user
    user = data.user

    -- Get CSRF token
    userToken = DrupalPublish.getUserToken( props )

    if not userToken then
      LrErrors.throwUserError( 'Unable to get user token.' )
    end

  end

  return user, userToken

end

DrupalPublish.loadNode = function ( props, nid )

  local body, response = LrHttp.get( props.url .. 'lightroom/node/' .. nid, headers )
  local node = JSON.decode(body)
  return node

end

DrupalPublish.saveNode = function ( props, node )

  if node.nid then

      -- Update node
    local body, response = LrHttp.post(props.url .. 'lightroom/node/' .. node.nid, JSON.encode(node), headers, 'PUT')

    if not (response.status == 200) then
      DrupalPublish.throwError( 'Unable to update node.', body )
    end

  else

    -- Create node
    local body, response = LrHttp.post(props.url .. 'lightroom/node', JSON.encode(node), headers)

    if not (response.status == 200) then
      DrupalPublish.throwError( 'Unable to create node.', body )
    end

    node = JSON.decode(body)

  end

  return node

end

DrupalPublish.getCollectionBehaviorInfo = function ( publishSettings )

	return {
		defaultCollectionName = "New Collection",
		defaultCollectionCanBeDeleted = true,
		canAddCollection = true,
		maxCollectionSetDepth = 0,
		  -- Disable collection sets
	}

end

DrupalPublish.throwError = function (message, body)

  local data = JSON.decode(body)
  if data then
    message = message .. '\n' .. table.concat(data, '\n')
  end
  LrErrors.throwUserError( message )

end

DrupalPublish.getCommentsFromPublishedCollection = function ( publishSettings, arrayOfPhotoInfo, commentCallback )
end

DrupalPublish.getCommentsFromPublishedCollection = function ( publishSettings, arrayOfPhotoInfo, commentCallback )
end

DrupalPublish.getRatingsFromPublishedCollection = function ( publishSettings, arrayOfPhotoInfo, ratingCallback )
end

DrupalPublish.startDialog = function( props )

end

DrupalPublish.endDialog = function( props, why )

end

DrupalPublish.sectionsForBottomOfDialog = function( viewFactory, propertyTable )

  local share = LrView.share
  local bind = LrView.bind
	local result = {

		{
			title = 'Drupal Settings',
			synopsis = bind { key = 'url', object = propertyTable },

			viewFactory:row {
				viewFactory:static_text {
					title = 'URL',
					alignment = 'right',
					width = share 'labelWidth'
				},

				viewFactory:edit_field {
				  value = bind 'url',
					fill_horizontal = 1
				}

			},
			viewFactory:row {
			  viewFactory:static_text {
			    title = 'User Name',
			    alignment = 'right',
			    width = share 'labelWidth',
			  },
			  viewFactory:edit_field {
			    value = bind 'username',
			    immediate = true,
			  },
			},
			viewFactory:row {
			  viewFactory:static_text {
			    title = 'Password',
			    alignment = 'right',
			    width = share 'labelWidth',
			  },
			  viewFactory:password_field {
			    value = bind 'password',
			    immediate = true,
			  },
			},
		},
	}

	return result

end

DrupalPublish.viewForCollectionSettings = function( f, props, info )

  local publishedCollection = info.publishedCollection;
	local collectionSettings = assert( info.collectionSettings )

	-- Fill in default parameters. This code sample targets a hypothetical service
	-- that allows users to enable or disable ratings and comments on a per-collection
	-- basis.

	local user, userToken = DrupalPublish.userLogin(props)

	local body, response = LrHttp.post(props.url .. 'lightroom/collection/types', '', headers)
  local data = JSON.decode(body)

	if not (response.status == 200) then
    LrDialogs.showError( 'Unable to get node types.' )
    return
	end
	if not (data) then
	  LrDialogs.showError( 'Unable to read node types.' )
  end

	local types = {}
	local title = ' '
  for i, item in ipairs(data) do
    table.insert(types, {title = item.name, value = item.type})

    if item.type == collectionSettings.type then
      title = item.name
    end

  end

  -- todo types empty, throw error
  if #types == 0 then
    LrDialogs.showError( 'No collection node types configured. You must add \'field_collection_images\' to at least one node type.' )
  end

  local isPublished = (publishedCollection and publishedCollection:getRemoteId()) and true or false
  if not isPublished then
    collectionSettings.type = types[1]['value']
  end

  local share = import 'LrView'.share
	local bind = import 'LrView'.bind

	return f:group_box {
    title = 'Node Settings',
		fill_horizontal = 1,
	  size = 'regular',
	  f:column {
      bind_to_object = assert( collectionSettings ),
      spacing = f:label_spacing(),
      size = 'small',
      f:row {
    		f:static_text {
    			title = 'Type:',
    			alignment = 'left',
    		},
  	    f:popup_menu {
  	      items = types,
  	      value = bind 'type',
  	      enabled = not isPublished,
  	    },
      },
      -- may want to get rid of this
      -- nodes shouldn't be published until they're populated
--			f:checkbox {
--				title = "Published",  -- this should be localized via LOC
--				value = bind 'status',
--				checked_value = 1,
--				unchecked_value = 0,
--				enabled = isPublished
--			},
	  }
	}

end

DrupalPublish.updateCollectionSettings = function( props, info )

  local publishedCollection = assert( info.publishedCollection )
	local collectionSettings = assert( info.collectionSettings )

  -- User login
  local user, userToken = DrupalPublish.userLogin( props )

  local remoteId = publishedCollection:getRemoteId()

	if not ( remoteId ) then

    local node = {
		  uid = user.uid,
		  type = collectionSettings.type,
		  title = info.name,
      status = 0, -- don't publish an empty collection
      field_collection_images = { und = {} },
		}

    -- Create presentation
    node = DrupalPublish.saveNode( props, node )

    -- todo catch error here

    publishedCollection.catalog:withWriteAccessDo( "Set Remote ID", function()
      publishedCollection:setRemoteId( node.nid )
    end)

  else

    local node = DrupalPublish.loadNode( props, remoteId )

    node = {
      nid = node.nid,
      title = info.name,
    }

    node = DrupalPublish.saveNode( props, node )

    -- todo catch error here

	end

end

DrupalPublish.processRenderedPhotos = function( functionContext, exportContext )

	local props = exportContext.propertyTable

	-- Make a local reference to the export parameters.
	local exportSession = exportContext.exportSession

	-- Set progress title.
	local numPhotos = exportSession:countRenditions()
	local progressScope = exportContext:configureProgress {
		title = numPhotos > 1 and string.format("Publishing %d photos.", numPhotos) or "Publishing 1 photo."
	}

  -- Get collection info
  local publishedCollection = exportContext.publishedCollection
  local publishedCollectionInfo = exportContext.publishedCollectionInfo

  -- User login
  local user, userToken = DrupalPublish.userLogin(props)

  local progressScope = exportContext:configureProgress {
		title = "Uploading",
	}

  local node = DrupalPublish.loadNode( props, publishedCollectionInfo.remoteId )

  if not ( node.field_collection_images and node.field_collection_images.und ) then
    node.field_collection_images = { und = {} }
  end

  -- Get rid of the extra stuff that Drupal returns
  for i, v in pairs(node.field_collection_images.und) do
    node.field_collection_images.und[i] = { fid = v.fid }
  end

  node = {
    nid = node.nid,
    title = node.title,
    status = node.status,
    promote = node.promote,
    field_collection_images = node.field_collection_images,
  }

	for i, rendition in exportContext:renditions{ stopIfCanceled = true } do

		-- Wait for next photo to render.
		local success, filePath = rendition:waitForRender()

		-- Check for cancellation again after photo has been rendered.
		if progressScope:isCanceled() then
		  return
		end

		if success then

			local fileName = LrPathUtils.leafName( filePath )

			-- Upload the new file
      local body, response = LrHttp.postMultipart( props.url .. 'lightroom/file/create_raw',
        {
          {
            name = 'files[]',
            fileName = fileName,
            filePath = filePath,
            contentType = 'application/octet-stream'
          }
        },
        {
          { field = 'Accept', value = 'application/json' },
          { field = 'X-CSRF-Token', value = userToken },
        }
      )

      -- Handle errors
      if not (response.status == 200) then
        logger:trace(body)
        -- log the error
        -- continue or cancel?
        LrErrors.throwUserError( 'Unable to upload file.' )

      end

      local file = JSON.decode(body)
      local fid = file[1].fid

      -- Update metadata
      local caption = rendition.photo:getFormattedMetadata('caption')
      local metadata = {
        field_caption = { und = { { value = caption } } }
      }

      LrHttp.post( props.url .. 'lightroom/file/' .. fid .. '/metadata', JSON.encode(metadata), headers )

		  if rendition.publishedPhotoId then
        -- Update existing files
		    for i, v in pairs(node.field_collection_images.und) do
		      if v.fid == rendition.publishedPhotoId then
		        node.field_collection_images.und[i] = { fid = fid }
		      end
		    end

		  else
		    -- Insert a new file
		    table.insert(node.field_collection_images.und, { fid = fid } )
      end

      -- Set the remote ID for this rendition
			rendition:recordPublishedPhotoId( fid )

      -- Save node
      node = DrupalPublish.saveNode( props, node )

		end

  end

  exportSession:recordRemoteCollectionId( node.nid )

end

DrupalPublish.imposeSortOrderOnPublishedCollection = function( props, info, remoteIdSequence )

  if info.remoteCollectionId then

    DrupalPublish.userLogin(props)

    local node = DrupalPublish.loadNode(props, info.remoteCollectionId)
    -- Reset the field
    node = {
      nid = node.nid,
      field_collection_images = { und = {} }
    }

    -- Update the order of images
    for i, fid in pairs(remoteIdSequence) do
    	table.insert(node.field_collection_images.und, { fid = fid })
    end

    -- Save node
    DrupalPublish.saveNode(props, node)

  end

end

DrupalPublish.renamePublishedCollection = function( props, info )

	if info.remoteId then

    DrupalPublish.userLogin(props)

    local node = DrupalPublish.loadNode(props, info.remoteId)
    -- Update the node title
    node = {
      nid = node.nid,
      title = info.name,
    }

    -- Save node
    DrupalPublish.saveNode(props, node)

	end

end

return DrupalPublish
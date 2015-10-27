/**
 * @file
 * Models / Controllers for the Getty Images plugin.
 *
 * @package Getty Images
 *
 * @author bendoh, thinkoomph
 */

(function($) {
  var g = GettyImages, s = getty_s, Attachments;

  // Simple Getty API module. Relies on CORS support.
  var api = {
    id: '10787',
    key: 'fQGtDPrif9akkGsXkde0gC30E1JdUpmWFOuMQxkF+xI=',
    anonymous: {
      username: 'wordpressplugin_api',
      password: 'vNua0JpFtpWe3lQ'
    },
    endpoints: {
      CreateSession: 'https://connect.gettyimages.com/v1/session/CreateSession',
      CreateApplicationSession: 'https://connect.gettyimages.com/v1/session/CreateApplicationSession',
      RenewSession: 'https://connect.gettyimages.com/v1/session/RenewSession',
      GetActiveProductOfferings: 'http://connect.gettyimages.com/v1/data/GetActiveProductOfferings',
      GetAllProductOfferings: 'http://connect.gettyimages.com/v1/data/GetAllProductOfferings',
      SearchForImages: 'http://connect.gettyimages.com/v2/search/SearchForImages',
      GetImageDetails: 'http://connect.gettyimages.com/v1/search/GetImageDetails',
      GetLargestImageDownloadAuthorizations: 'http://connect.gettyimages.com/v1/download/GetLargestImageDownloadAuthorizations',
      GetImageDownloadAuthorizations: 'http://connect.gettyimages.com/v1/download/GetImageDownloadAuthorizations',
      CreateDownloadRequest: 'https://connect.gettyimages.com/v1/download/CreateDownloadRequest',
      GetEventDetails: 'http://connect.gettyimages.com/v1/search/GetEventDetails'
    },

    // Make an API request, return a promise for the request.
    request: function(endpoint, body) {
      var payload = {
        RequestHeader: {
          CoordinationId: '',
          Token: ''
        }
      };

      var url = api.endpoints[endpoint];
      if(!url) {
        return;
      }

      // Have we a session?
      var session = g.user.get('session');

      // Is it valid?
      if(session && session.expires < new Date().getTime() / 1000) {
        g.user.logout();
        session = false;
      }

      // Assume that the request body name is the same as the endpoint name +
      // "RequestBody", as appears to be the case for almost all of the API calls,
      // except "CreateDownloadRequest".
      if(endpoint == 'CreateDownloadRequest') {
        payload['CreateDownloadRequestBody'] = body;
      }
      else if(endpoint == 'SearchForImages') {
        payload['SearchForImagesRequestBody'] = body;
      }
      else {
        payload[endpoint + 'RequestBody'] = body;
      }

      // Defer the request until we get authentication.
      var result = $.Deferred();
      var defer = $.Deferred();

      defer
        .done(function() {
          // Even if we have a valid session, providing it as a token
          // to CreateSession results in an error.
          if(endpoint != 'CreateSession' && endpoint != 'CreateApplicationSession') {
            var tokens = session ? session : g.user.anonymous;

            // Choose correct session token based on request protocol.
            if(url.match(/^https:/)) {
              payload.RequestHeader.Token = tokens.secure;
            }
            else {
              payload.RequestHeader.Token = tokens.token;
            }

            if(!payload.RequestHeader.Token) {
              result.reject({ Message: "No valid token found" });
            }
          }

          $.ajax(url, {
            data: JSON.stringify(payload),
            type: 'POST',
            accepts: 'application/json',
            contentType: 'application/json',
          })
            .fail(result.reject)
            .done(function(data) {
              if(data.ResponseHeader.Status == 'error') {
                result.reject(data.ResponseHeader.StatusList);
              }
              else {
                result.resolve(data[endpoint + 'Result']);
              }
            });
        })
        .fail(function() {
          // API must be unavailable. Messaging!
          result.reject();
        });

      // Try unauthenticated credentials for requests with no session or https.
      if(!session && url.match(/^http:/)) {
        g.user.createApplicationSession()
          .done(defer.resolve)
          .fail(defer.reject);
      }
      else {
        defer.resolve();
      }

      return result.promise();
    }
  };

  // A single Getty Image result.
  g.model.Attachment = Backbone.Model.extend({
    parse: function(resp, xhr) {
      if (! resp) {
        return resp;
      }

      var dateRegexp = /^\/Date\((\d+)(-\d+)?\)\/$/;

      this.set('sizes', g.sizes);

      // Convert date strings into Date objects.
      _.each([ 'DateCreated', 'DateSubmitted' ], function(field) {
        var match = resp[field].match(dateRegexp);

        if(match) {
          resp[field] = new Date(parseInt(match[1])).toString();
        }
      });

      return resp;
    },

    sync: function(method, model, options) {
      if(method == 'read') {
        this.getDrupalDetails();

        // Clone the args so manipulation is non-destructive.
        var args = _.clone(this.args);

        this.set('downloadingDetails', true);

        var self = this;
        return api.request('GetImageDetails', {
          CountryCode: 'USA',
          ImageIds: [ model.get('ImageId') ]
        })
          .done(function(data) {
            self.set('haveDetails', true);
            self.set(self.parse(data.Images[0], this));

            if(g.user.get('loggedIn')) {
              self.getDownloadAuthorizations();
            }
          })
          .fail(function(data) {
            // TODO: Error condition.
          })
          .always(function() {
            self.unset('downloadingDetails');
          });
      }
    },

    hasDownloads: function() {
      return this.get('files') && this.get('files').length > 0;
    },

    getDownloadAuthorizations: function() {
      var sizes = this.get('SizesDownloadableImages');

      // No downloadable image sizes anyway. Do nothing.
      if(!sizes) {
        return;
      }

      var request = {
        ImageSizes: _.map(sizes, function(size) {
          return {
            ImageId: this.get('ImageId'),
            SizeKey: size.SizeKey
          }
        }, this)
      };

      sizes = _.groupBy(sizes, 'SizeKey');

      this.set('authorizing', true);

      // Get download authorizations for all sizes.
      var self = this;

      api.request('GetImageDownloadAuthorizations', request)
        .done(function(data) {
          var authorizations = [];

          // De-normalize authorizations and size against image data, turning
          // each image into a flat structure.
          _.each(data.Images, function(image) {
            if(image.ImageId == self.get('ImageId') && image.Status == 'available') {
              _.each(image.Authorizations, function(auth) {
                var imageAuth = _.extend(image, sizes[image.SizeKey][0]);
                imageAuth = _.extend(imageAuth, auth);
                authorizations.push(imageAuth);
              });

              delete image.Authorizations;
            }
          });

          self.set('DownloadAuthorizations', authorizations);
        })
        .always(function() {
          self.unset('authorizing');
        });
    },

    download: function() {
      var downloadToken = this.get('DownloadToken');

      if(downloadToken) {
        return this.downloadWithToken(downloadToken);
      }

      return $.Deferred.reject().promise();
    },

    // Download a single size of a single image given a download token.
    downloadWithToken: function(token) {
      var self = this;

      this.set('downloading', true);

      return api.request('CreateDownloadRequest', {
        DownloadItems: [ { DownloadToken: token } ]
      });
    },

    // Image downloaded! Yay!
    downloaded: function(response) {
      this.set('files', new Backbone.Collection(response));

      // Clear out any cached queries in media library.
      if(s) {
        s.events = 'event6';
        s.prop1 = s.eVar1 = s.prop2 = s.eVar2 = '';
        s.prop3 = s.eVar3 = this.get('ImageId');
        s.tl();
      }
    },

    // Get Drupal image details.
    getDrupalDetails: function() {
      // Get the URL, forward for sideloading.
      var self = this;
      g.post('details/' + this.get('ImageId'))
        .done(function(response) {
          if(response.success) {
            self.set('files', new Backbone.Collection(response.data));
          }
        });
    }
  }, {
    create: function(attrs) {
      return Attachments.all.push(new g.model.Attachment(attrs));
    },

    get: _.memoize(function(id, attachment) {
      return Attachments.all.push(attachment || new g.model.Attachment({ id: id }));
    })
  });

  // The collection of Getty Images search results.
  Attachments = g.model.Attachments = Backbone.Collection.extend({
    model: g.model.Attachment,

    /**
     * Automatically sort the collection when the order changes.
     */
    _changeOrder: function() {
      if (this.comparator) {
        this.sort();
      }
    },

    /**
     * Set the default comparator only when the `orderby` property is set.
     *
     * @param {Backbone.Model} model
     * @param {string} orderby
     */
    _changeOrderby: function(model, orderby) {
      // If a different comparator is defined, bail.
      if (this.comparator && this.comparator !== Attachments.comparator) {
        return;
      }

      if (orderby && 'post__in' !== orderby) {
        this.comparator = Attachments.comparator;
      } else {
        delete this.comparator;
      }
    },

    /**
     * If the `query` property is set to true.
     *
     * Query the server using the `props` values,
     * and sync the results to this collection.
     *
     * @param {Backbone.Model} model
     * @param {Boolean} query
     */
    _changeQuery: function(model, query) {
      if (query) {
        this.props.on('change', this._requery, this);
        this._requery();
      } else {
        this.props.off('change', this._requery, this);
      }
    },
    /**
     * If this is a query, updating the collection will be handled.
     *
     * By `this._requery()`.
     *
     * @param {Backbone.Model} model
     */
    _changeFilteredProps: function(model) {
      if (this.props.get('query')) {
        return;
      }

      var changed = _.chain(model.changed).map(function(t, prop) {
        var filter = Attachments.filters[ prop ],
          term = model.get(prop);

        if (! filter) {
          return;
        }

        if (term && ! this.filters[ prop ]) {
          this.filters[ prop ] = filter;
        } else if (! term && this.filters[ prop ] === filter) {
          delete this.filters[ prop ];
        } else {
          return;
        }

        // Record the change.
        return true;
      }, this).any().value();

      if (! changed) {
        return;
      }

      // If no `Attachments` model is provided to source the searches
      // from, then automatically generate a source from the existing
      // models.
      if (! this._source) {
        this._source = new Attachments(this.models);
      }

      this.reset(this._source.filter(this.validator, this));
    },

    validateDestroyed: false,
    /**
     * Validate attachment model against all filters.
     *
     * @param {wp.media.model.Attachment} attachment
     *
     * @returns {Boolean}
     */
    validator: function(attachment) {
      if (! this.validateDestroyed && attachment.destroyed) {
        return false;
      }
      return _.all(this.filters, function(filter) {
        return !! filter.call(this, attachment);
      }, this);
    },
    /**
     * Validate and add/remove attachment model based on the result.
     *
     * @param {wp.media.model.Attachment} attachment
     * @param {Object} options
     *
     * @returns {wp.media.model.Attachments} Returns itself to allow chaining
     */
    validate: function(attachment, options) {
      var valid = this.validator(attachment),
        hasAttachment = !! this.get(attachment.cid);

      if (! valid && hasAttachment) {
        this.remove(attachment, options);
      } else if (valid && ! hasAttachment) {
        this.add(attachment, options);
      }

      return this;
    },

    /**
     * Validate all attachments.
     *
     * @param {wp.media.model.Attachments} attachments
     * @param {object} [options={}]
     *
     * @fires wp.media.model.Attachments#reset
     *
     * @returns {wp.media.model.Attachments} Returns itself to allow chaining
     */
    validateAll: function(attachments, options) {
      options = options || {};

      _.each(attachments.models, function(attachment) {
        this.validate(attachment, { silent: true });
      }, this);

      if (! options.silent) {
        this.trigger('reset', this, options);
      }
      return this;
    },

    /**
     * Observe another collection of attachments.
     *
     * @param {wp.media.model.Attachments} attachments
     *
     * @returns {wp.media.model.Attachments} Returns itself to allow chaining
     */
    observe: function(attachments) {
      attachments.results.on('change', this.syncResults, this);

      this.observers = this.observers || [];
      this.observers.push(attachments);

      attachments.on('add change remove', this._validateHandler, this);
      attachments.on('reset', this._validateAllHandler, this);
      this.validateAll(attachments);
      return this;
    },

    /**
     * Stop observating an attachment collection.
     *
     * @param {wp.media.model.Attachments} attachments
     *
     * @returns {wp.media.model.Attachments} Returns itself to allow chaining
     */
    unobserve: function(attachments) {
      attachments.results.off('change', this.syncResults, this);

      if (attachments) {
        attachments.off(null, null, this);
        this.observers = _.without(this.observers, attachments);

      } else {
        _.each(this.observers, function(attachments) {
          attachments.off(null, null, this);
        }, this);
        delete this.observers;
      }

      return this;
    },

    /**
     * Execute validations on an attachment when it is added or removed from its collection.
     *
     * @param {wp.media.model.Attachments} attachment
     * @param {wp.media.model.Attachments} attachments
     * @param {Object} options
     *
     * @access private
     *
     * @returns {wp.media.model.Attachments} Returns itself to allow chaining
     */
    _validateHandler: function(attachment, attachments, options) {
      // If we're not mirroring this `attachments` collection,
      // only retain the `silent` option.
      options = attachments === this.mirroring ? options : {
        silent: options && options.silent
      };

      return this.validate(attachment, options);
    },

    /**
     * Validate all attachments.
     *
     * @param {wp.media.model.Attachments} attachments
     * @param {Object} options
     *
     * @returns {wp.media.model.Attachments} Returns itself to allow chaining
     *
     * @access private
     */
    _validateAllHandler: function(attachments, options) {
      return this.validateAll(attachments, options);
    },

    /**
     * Mirror another collection. Reset the current collection and observe the given collection.
     *
     * @param {wp.media.model.Attachments} attachments
     *
     * @returns {wp.media.model.Attachments} Returns itself to allow chaining
     */
    mirror: function(attachments) {
      if (this.mirroring && this.mirroring === attachments) {
        return this;
      }

      this.unmirror();
      this.mirroring = attachments;

      // Clear the collection silently. A `reset` event will be fired
      // when `observe()` calls `validateAll()`.
      this.reset([], { silent: true });
      this.observe(attachments);

      return this;
    },
    unmirror: function() {
      if (! this.mirroring) {
        return;
      }

      this.unobserve(this.mirroring);
      delete this.mirroring;
    },

    initialize: function(models, options) {
      options = options || {};

      // Queried search properties.
      this.props = new Backbone.Model();

      // Queue up query changes in this model, but don't refresh search
      // until executed.
      this.propsQueue = new Backbone.Model();

      // Result data: total, refinements.
      this.results = new Backbone.Model();

      // Propagate changes to props to propsQueue.
      this.props.on('change', this.changeQueuedProps, this);
    },

    // Forward changes to search props to the queue.
    changeQueuedProps: function() {
      this.propsQueue.set(this.props.attributes);
    },

    // Sync up results with mirrored query.
    syncResults: function(model) {
      this.results.set(model.changed);
    },

    // Perform a search with queued search properties.
    search: function() {
      var searchTerm = this.propsQueue.get('search');

      if(typeof searchTerm != 'string' || !searchTerm.match(/[^\s]/)) {
        return;
      }

      if(this.propsQueue.get('Refinements') && this.propsQueue.get('Refinements').length == 0) {
        this.propsQueue.unset('Refinements');
      }

      if(g.user.settings.get('mode') == 'embed') {
        this.propsQueue.set('EmbedContentOnly', "true");
      }
      else {
        this.propsQueue.unset('EmbedContentOnly');
      }

      var query = g.model.Query.get(this.propsQueue.toJSON());

      if(query !== this.mirroring) {
        this.reset();
        this.props.clear();
        this.props.set(this.propsQueue.attributes);

        this.results.unset('total');
        this.results.set('searching', true);
        this.results.set('searched', false);

        this.mirror(query);

        if(s) {
          s.events = 'event2';
          s.prop1 = s.eVar1 = s.prop3 = s.eVar3;
          s.prop2 = s.eVar2 = this.props.get('search');

          var imageFamily = this.props.get('ImageFamilies');
          // There has to be a better way!
          imageFamily = imageFamily.charAt(0).toUpperCase() + imageFamily.slice(1);

          var sortOrder = this.props.get(imageFamily + 'SortOrder');
          var orientations = this.props.get('Orientations').join(',');
          var excludeNudity = this.props.get('ExcludeNudity') ? 'Exclude' : '';

          s.prop6 = s.eVar6 = [ imageFamily, sortOrder, orientations, excludeNudity ].join('|');

          s.tl();
        }
      }

      // Force reset of attributes for cached queries.
      if(query.cached) {
        this.results.clear();
        this.results.set(query.results.attributes);
      }
    },

    // Grab more from the mirrored query.
    more: function(options) {
      var deferred = $.Deferred(),
        mirroring = this.mirroring,
        attachments = this;

      if (! mirroring || ! mirroring.more) {
        return deferred.resolveWith(this).promise();
      }
      // If we're mirroring another collection, forward `more` to
      // the mirrored collection. Account for a race condition by
      // checking if we're still mirroring that collection when
      // the request resolves.
      mirroring.more(options).done(function() {
        if (this === attachments.mirroring) {
          deferred.resolveWith(this);
        }
      });

      return deferred.promise();
    },
  }, {
    filters: {
      search: function(attachment) {
        if (!this.props.get('search')) {
          return true;
        }

        return _.any(['Title', 'Caption', 'ImageId', 'UrlComp', 'ImageFamily'], function(key) {
          var value = attachment.get(key);
          return value && - 1 !== value.search(this.props.get('search'));
        }, this);
      }
    }
  });

  // Cache all attachments here. TODO: Memory clean up?
  Attachments.all = new g.model.Attachments();

  var EventRefinements = new Backbone.Collection();

  // The Getty Images query and parsing model.
  g.model.Query = Backbone.Collection.extend({
    initialize: function(models, options) {
      // Track refinement options and total.
      this.results = new Backbone.Model();

      this.props = new Backbone.Model(options.props);
      this.args = options.args;

      this._hasMore = true;

      this.results.unset('total');
      this.results.set('searching', false);
      this.results.set('searched', false);

      // Track number of results returned from API separately from
      // the size of the collection, as the API MAY give back duplicates
      // which will cause the paging to screw up since the collection
      // will have fewer results than were actually returned.
      this.numberResults = 0;
    },

    hasMore: function() {
      return this._hasMore;
    },

    // Override more() to return a more-deferred deferred object
    // and not bother trying to use Backbone sync() or fetch() methods
    // to get the data, since this is a very custom workflow.
    more: function(options) {
      if (this._more && 'pending' === this._more.state()) {
        return this._more;
      }

      if(!this.hasMore()) {
        return $.Deferred().resolveWith(this).promise();
      }

      if(_.isEmpty(this.props.get('search'))) {
        return $.Deferred().resolveWith(this).promise();
      }

      // Flag the search as executing.
      this.results.set('searching', true);

      // Build searchPhrase from any main query + refinements.
      var searchPhrase = this.props.get('search');

      if(this.props.get('SearchWithin')) {
        searchPhrase += ' ' + this.props.get('SearchWithin');
      }

      var args = _.clone(this.args);
      this.page = Math.floor(this.numberResults / args.posts_per_page);
      var request = {
        Query: {
          SearchPhrase: searchPhrase,
        },
        ResultOptions: {
          IncludeKeywords: false,
          ItemCount: args.posts_per_page,
          ItemStartNumber: this.page * args.posts_per_page + 1,
          EditorialSortOrder: this.props.get('EditorialSortOrder'),
          CreativeSortOrder: this.props.get('CreativeSortOrder')
        },
        // Could write this much more compactly, but expanding it for clarity.
        Filter: {
          // Note that this value expects array but we only ever pass one value.
          ImageFamilies: [ this.props.get('ImageFamilies') ],
          GraphicStyles: this.props.get('GraphicStyles'),
          Orientations: this.props.get('Orientations'),
          ExcludeNudity: this.props.get('ExcludeNudity'),
          Refinements: this.props.get('Refinements'),
          EmbedContentOnly: this.props.get('EmbedContentOnly')
        }
      };

      // Make logical tweaks to filter based on what the API can do.
      if(this.props.get('ImageFamilies') == 'editorial') {
        delete request.ResultOptions.CreativeSortOrder;
        // Can't choose Graphic Style for editorial, since only photography
        // is available.
        delete request.Filter.GraphicStyles;
      }
      else if(this.props.get('ImageFamilies') == 'creative') {
        // Editorial Sort Order doesn't make any sense for creative.
        delete request.ResultOptions.EditorialSortOrder;
      }

      // Get refinement options for the first page of results only.
      if(this.page == 0) {
        request.ResultOptions.RefinementOptionsSet = 'RefinementSet2';
      }

      // Add in any applicable product offerings so the user only gets
      // results for product offerings he owns.
      var products = g.user.get('products');

      if(products && products.length) {
        request.Filter.ProductOfferings = products;
      }

      // Proxy the deferment from the API query so we can retry if necessary.
      if('deferred' in this) {
        this.deferred.retry++;
      }
      else {
        this.deferred = $.Deferred();
        this.deferred.retry = 0;
      }

      var self = this;
      this._more = api.request('SearchForImages', request)
        .done(function(response) {
          self.set(self.parse(response), { remove: false });

          self.numberResults += response.Images.length;

          if(response.Images.length == 0 || response.Images.length < args.posts_per_page) {
            self._hasMore = false;
          }

          self.deferred.resolve(response);
          delete self.deferred;
        })
        .fail(function(response) {
          if(self.deferred.retry < 3) {
            self.more();
          }
          else {
            self.deferred.reject(response);
            delete self.deferred;
          }
        })
        .always(function() {
          self.results.set('searching', false);
        });

      return (this.deferred ? this.deferred : $.Deferred().reject()).promise();
    },

    parse: function(response, xhr) {
      this.results.set('total', response.ItemTotalCount);

      if(this.page == 0 && response.RefinementOptions && typeof response.RefinementOptions == "object") {
        var refinements = new Backbone.Collection();
        var eventRefinementQueue = [];

        var refinementCategories = _.groupBy(response.RefinementOptions, 'Category');

        // Remove "AssetFamily" category because it's already a primary search
        // parameter.
        delete refinementCategories.AssetFamily;
        // Remove "PhotographerName" category because of API issues (See GETTY-37).
        delete refinementCategories.PhotographerName;

        _.each(refinementCategories, function(options, category) {
          var categoryModel = new Backbone.Model({
            id: category,
            options: new Backbone.Collection()
          });

          refinements.add(categoryModel);

          _.each(options, function(option) {
            var optionModel = new Backbone.Model({
              id: option.Id,
              text: option.Text,
              category: category,
              count: option.ImageCount
            });

            categoryModel.get('options').add(optionModel);

            if(category == "Event") {
              var eventRefinement = EventRefinements.get(optionModel.id);

              // Check if this is a known event.
              if(!eventRefinement || !eventRefinement.get('event')) {
                // If not, add event ID to lookup queue and save
                // reference in cache.
                EventRefinements.add(optionModel);

                eventRefinementQueue.push(optionModel.id);
              }
              else {
                // If so, update text and set event reference.
                optionModel.set('text', eventRefinement.get('text'));
                optionModel.set('event', eventRefinement.get('event'));
              }
            }
          });
        });

        this.results.set('refinements', refinements);

        if(eventRefinementQueue.length) {
          // Get any unknown event IDs from the API.
          api.request('GetEventDetails', {
            EventIds: eventRefinementQueue
          })
            .done(function(result) {
              _.each(result.EventResult, function(evt) {
                var eventRefinement = EventRefinements.get(evt.EventId);

                if(eventRefinement) {
                  eventRefinement.set('text', evt.Event.EventName);
                  eventRefinement.set('event', evt.Event);
                }
              });
            });
        }
      }

      this.results.set('searched', true);

      var attachments = _.map(response.Images, function(item) {
        var id, attachment, newAttributes;

        id = item.ImageId;

        attachment = g.model.Attachment.get(id);
        newAttributes = attachment.parse(item, xhr);

        if (! _.isEqual(attachment.attributes, newAttributes)) {
          attachment.set(newAttributes);
        }

        return attachment;
      });

      this.getImageDownloadAuths(attachments);

      return attachments;
    },

    getImageDownloadAuths: function(attachments) {
      // Get the set of images which don't already have
      // known DownloadAuths.
      var images = _.map(_.filter(attachments, function(attachment) {
        return !attachment.get('LargestDownloadAuthorizations')
      }), function(attachment) {
        return { ImageId: attachment.get('ImageId') };
      });

      // Why bother with an empty set?
      if(images.length == 0) {
        return;
      }

      return api.request('GetLargestImageDownloadAuthorizations', {
        Images: images
      })
        .done(function(response) {
          _.each(response.Images, function(auth) {
            var model = g.model.Attachments.all.get(auth.ImageId);

            model.set('Status', auth.Status);

            if(auth.Authorizations.length > 0) {
              if(!model.get('ProductOffering')) {
                model.set('ProductOffering', auth.Authorizations[0].ProductOfferingType);
              }
              if(!model.get('DownloadSizeKey')) {
                model.set('DownloadSizeKey', auth.Authorizations[0].SizeKey);
              }

              model.set('LargestDownloadAuthorizations', auth.Authorizations);
            }
          });
        });
    }
  }, {
    defaultArgs: {
      posts_per_page: 50
    }
  });

  // Ensure our query object gets used instead, there's no other way
  // to inject a custom query object into g.model.Query.get
  // so we must override. This function caches distinct queries
  // so that re-queries come back instantly. Though there's no memory cleanup.
  g.model.Query.get = (function() {
    var queries = [];
    var Query = g.model.Query;

    return function(props, options) {
      var args     = {},
          defaults = Query.defaultProps,
          query;

      // Remove the `query` property. This isn't linked to a query,
      // this *is* the query.
      delete props.query;

      // Fill default args.
      _.defaults(props, defaults);

      // Generate the query `args` object.
      // Correct any differing property names.
      _.each(props, function(value, prop) {
        if(_.isNull(value)) {
          return;
        }

        args[prop] = value;
      });

      // Fill any other default query args.
      _.defaults(args, Query.defaultArgs);

      // Search the query cache for matches.
      query = _.find(queries, function(query) {
        return _.isEqual(query.args, args);
      });

      // Otherwise, create a new query and add it to the cache.
      if (! query) {
        query = new Query([], _.extend(options || {}, {
          props: props,
          args:  args
        }));

        // Only push successful queries into the cache.
        query.more().done(function() {
          queries.push(query);
        });
      }
      else {
        // Flag that this was a cached query.
        query.cached = true;

        // Reverse the models for cached queries
        // because there's a bug in the media library
        // that causes the images to come back
        // in the reverse order when it's cached.
        query.models.reverse();
      }

      return query;
    };
  }());

  // Getty user and session management.
  g.model.User = Backbone.Model.extend({
    defaults: {
      id: 'getty-images-login',
      username: '',
      loginToken: '',
      seriesToken: '',
      loginTime: 0,
      loggedIn: false
    },

    initialize: function() {
      var settings = {};

      try {
        settings = JSON.parse($.cookie('drupalGIc')) || {};
      } catch(ex) {
      }

      this.settings = new Backbone.Model(settings);
      this.settings.on('change', this.updateUserSettings, this);
    },

    updateUserSettings: function(model, values) {
      $.cookie('drupalGIc', JSON.stringify(model));
    },

    // Create an application-level session for anonymous searching,
    // return a promise object.
    createApplicationSession: function() {
      // If we couldn't log in, create or restore an anonymous session.
      // Refresh the session only if it's 1 minute before or any time after
      // expiration.
      var session = this.anonymous;

      if(!session || session.expires < new Date().getTime() / 1000 - 60) {
        // No session or most definitely timed out session, create a new one.
        var self = this;

        session = this.anonymous = { promise: $.Deferred() };

        api.request('CreateApplicationSession', {
          SystemId: api.id,
          SystemPassword: api.key
        })
          .done(function(result) {
            session.token = result.Token;
            session.expires = parseInt(result.TokenDurationMinutes * 60) + parseInt(new Date().getTime() / 1000);

            session.promise.resolve();
          })
          .fail(function(result) {
            session.promise.reject()
          });
      }

      return session.promise;
    },

    // Restore login session from cookie.  Sets loggedIn if the session
    // expiration has not passed yet.
    restore: function() {
      var loginCookie = jQuery.cookie('drupalGIs');

      this.unset('error');

      var loggedIn = false;

      if(loginCookie && loginCookie.indexOf(':') > -1) {
        var un_token_series = loginCookie.split(':');

        if(un_token_series.length == 4) {
          this.set('username', un_token_series[0]);

          this.set('session', {
            token: un_token_series[1],
            secure: un_token_series[2],
            expires: un_token_series[3]
          });

          // Consider ourselves logged in if expiration hasn't passed.
          // If less than 5 minutes away, try to refresh from server.
          var diff = un_token_series[3] - new Date().getTime() / 1000;

          if(diff > 0) {
            loggedIn = true;

            // Pull in available product offerings for the user.
            var self = this;
            api.request('GetActiveProductOfferings')
              .done(function(result) {
                self.set('products', result.ActiveProductOfferings);
                self.set('ProductOffering', result.ActiveProductOfferings[0]);
              });

            // Refresh session on a timer to keep it alive,
            // either immediately or when the session is 2 minutes from expiring.
            //
            // refresh() will call restore() on success, restore() will re-set the timer
            // if the refresh was successful.
            var self = this;

            this.refreshTimer = setTimeout(function() {
              self.refresh();
            }, Math.max(diff - 1 * 60, 0) * 1000);
          }
        }
      }

      this.set('loggedIn', loggedIn);
    },

    login: function(password) {
      this.set('loggingIn', true);
      this.unset('error');

      var self = this;

      return api.request('CreateSession', {
        SystemId: api.id,
        SystemPassword: api.key,
        UserName: this.get('username'),
        UserPassword: password
      })
        .done(function(result) {
          // Stick the session data in a persistent cookie.
          self.refreshSession(result);

          if(s) {
            s.events = 'event1';
            s.prop2 = s.eVar2 = s.prop3 = s.eVar3 = '';
            s.prop1 = s.eVar1 = self.get('username');
            s.tl();
          }
        })
        .fail(function(statuses) {
          self.set('loggedIn', false);
          self.trigger('change:loggedIn');
          self.set('promptLogin', true);

          self.set('error', statuses[0] && statuses[0].Message || "API error");
        })
        .always(function() {
          self.set('loggingIn', false);
        });
    },

    refreshSession: function(result) {
      // Pluck the values we need to save the session.
      var session = [
        this.get('username'),
        result.Token,
        result.SecureToken,
        parseInt(result.TokenDurationMinutes * 60) + parseInt(new Date().getTime() / 1000)
      ];

      // Save it in a cookie: nom.
      $.cookie('drupalGIs', session.join(':'));

      // Restore from the cookie.
      this.restore();
    },

    // Try to refresh User session in the API.
    // Keep session in cookie.
    refresh: function() {
      clearTimeout(this.refreshTimer);
      delete this.refreshTimer;

      var self = this;

      // Return any existing restoration promise.
      if(self.refreshing) {
        return self.refreshing;
      }

      self.unset('error');
      self.set('loggingIn', true);

      self.refreshing = api.request('RenewSession', {
        SystemId: api.id,
        SystemPassword: api.key
      })
        .always(function() {
          delete self.refreshing;
          self.set('loggingIn', false);
        })
        .done(function(result) {
          // Stick the session data in a persistent cookie.
          self.refreshSession(result);
        })
        .fail(function(data) {
          self.set('error', data.message);
        });

      // Return actual promise object if we're less than 1 minute away
      // from expiration.
      if(self.get('loggedIn') && self.get('expires') - new Date().getTime() < 1 * 60 * 1000)  {
        return self.refreshing;
      }
      else {
        // Otherwise return affirmative immediately.
        return $.Deferred().resolveWith(this, {success: true}).promise();
      }
    },

    logout: function() {
      // Log out, which will clear (most) of the cookie values out
      // so the username can be retained but the login and session
      // tokens get erased.
      var self = this;

      // Throw away expired sessions.
      $.cookie('drupalGIs', '');

      // Throw away invalidated attributes.
      self.unset('session');
      self.unset('products');

      self.set('loggedIn', false);

      // Throw away all download auths and image statuses.
      g.model.Attachments.all.each(function(image) {
        image.unset('SizesDownloadableImages');
        image.unset('Authorizations');
        image.unset('Status');
      });

      // Trash cookie but keep the username for convenience.
      $.cookie('drupalGIs', [ self.get('username'), '', '', '' ].join(':'));

      // Use application session.
      this.createApplicationSession();
    }
  });

  // g.model.Selection
  // Used to manage a selection of attachments in the views.
  g.model.Selection = Attachments.extend({
    /**
     * Refresh the `single` model whenever the selection changes.
     *
     * Binds `single` instead of using the context argument to ensure
     * it receives no parameters.
     */
    initialize: function(models, options) {
      /**
       * Call 'initialize' directly on the parent class.
       */
      Attachments.prototype.initialize.apply(this, arguments);

      this.multiple = options && options.multiple;

      this.on('add remove reset', _.bind(this.single, this, false));
    },

    /**
     * Override the selection's add method.
     *
     * If the workflow does not support multiple
     * selected attachments, reset the selection.
     *
     * Overrides Backbone.Collection.add
     * Overrides wp.media.model.Attachments.add
     */
    add: function(models, options) {
      if (!this.multiple) {
        this.remove(this.models);
      }

      /**
       * Call 'add' directly on the parent class.
       */
      return Attachments.prototype.add.call(this, models, options);
    },

    /**
     * Triggered when toggling (clicking on) an attachment in the modal.
     */
    single: function(model) {
      var previous = this._single;

      // If a `model` is provided, use it as the single model.
      if(model) {
        this._single = model;
      }

      // If the single model isn't in the selection, remove it.
      if(this._single && !this.get(this._single.cid)) {
        delete this._single;
      }

      this._single = this._single || this.last();

      // If single has changed, fire an event.
      if(this._single !== previous) {
        if(previous) {
          previous.trigger('selection:unsingle', previous, this);

          // If the model was already removed, trigger the collection
          // event manually.
          if (!this.get(previous.cid)) {
            this.trigger('selection:unsingle', previous, this);
          }
        }

        if(this._single) {
          this._single.trigger('selection:single', this._single, this);
        }
      }

      // Return the single model, or the last model as a fallback.
      return this._single;
    }
  });

  // Display options based on an existing attachment.
  g.model.DisplayOptions = Backbone.Model.extend({
    initialize: function() {
      this.attachment = this.get('attachment');

      if(!this.attachment) {
        return;
      }

      this.attachment.on('change:files', function() {
        this.drupalImages = this.attachment.get('files');
        this.fetch();
      }, this);

      this.drupalImages = this.attachment.get('files');

      this.set('caption', this.attachment.get('Caption'));
      this.set('alt', this.attachment.get('Title'));

      this.set('sizes', _.clone(g.sizes));

      this.fetch();
    },

    sync: function(method, model, options) {
      if(method == 'read') {
        // If there's an attachment, pull the largest size in the database,
        // calculate potential sizes based on that.
        this.image = new Image();
        var url;

        if(!this.attachment.hasDownloads()) {
          url = this.attachment.get('UrlWatermarkComp');

          if(url == "unavailable") {
            url = this.attachment.get('UrlComp');
          }
        }
        else {
          // TODO : How to pick among multiple drupla downloads?
          url = this.drupalImages.at(0).get('url');
        }

        this.set('caption', this.attachment.get('Caption'));
        this.set('alt', this.attachment.get('Title'));

        $(this.image).on('load', this.loadImage());
        this.image.src = url;
      }
    },

    // Closure-in-closure because we can't control the binding of
    // 'this' with jQuery-registered event handlers.
    loadImage: function() {
      var self = this;

      return function(ev) {
        var sizes = {};
        var ar = this.width / this.height;

        // Constrain image to image sizes.
        _.each(g.sizes, function(size, slug) {
          var cr = size.width / size.height;
          var s = { label: size.label };

          s.url = this.src;

          if(ar > cr) {
            // Constrain to width.
            s.width = parseInt(size.width);
            s.height = parseInt(size.width / ar);
          }
          else {
            // Constrain to height or square!
            s.width = parseInt(ar * size.height);
            s.height = parseInt(size.height);
          }

          sizes[slug] = s;
        }, this);

        sizes.full = {
          label: g.text.fullSize,
          width: this.width,
          height: this.height,
          url: this.src
        }

        self.set('sizes', sizes);
      }
    }
  });
})(jQuery);

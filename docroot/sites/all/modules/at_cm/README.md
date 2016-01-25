AT CampaignMonitor integration
---

This module just provide an wrapper for `createsend-php`, the library help us
connect with CampaignMonitor service.

### Install

1. Install this module like any other Drupal modules: download it to ./sites/all/modules/
1. Run `drush atr` to download dependencies. If you do not have Drush available
    on your machine, download http://j.mp/N5gZ1x, unzip it to
    `sites/all/libraries/campaignmonitor`, then we can find
    `sites/all/libraries/campaignmonitor/csrest_general.php`
1. In your settings.php, add `$conf['at_cm.api_key'] = 'PUT YOUR API KEY HERE'`.
    This value is available at /admin/account/

### Usage

After success installation, cm servicers are available:

- cm.administrators
- cm.campaigns
- cm.clients
- cm.general
- cm.lists
- cm.people
- cm.segments
- cm.subscribers
- cm.templates

Example:

    at_container('cm.lists')->get_segments();

To have type hint, we can use AT_CM facade:

    AT_CM::getLists()->get_segments();

![Type hint](https://s3-ap-southeast-2.amazonaws.com/uploads-au.hipchat.com/36134/251454/DyYL2PcAgLFxVc6/cm_type_hints.gif)

### Drush commands

#### View Lists

    # List all lists
    drush at_cm.lists

    Client   Client ID                         List Name    List ID
    -------  -------                           -------      -------
    GO1      ***                               Sample List  ***

#### Explore API

`drush at_cm` command is available there for us to explore the API.

    $ drush at_cm

    Service            Class
    ----------         ---------------------
    cm.administrators  CS_REST_Administrators
    cm.campaigns       CS_REST_Campaigns
    cm.clients         CS_REST_Clients
    cm.general         CS_REST_General
    cm.lists           CS_REST_Lists
    cm.people          CS_REST_People
    cm.segments        CS_REST_Segments
    cm.subscribers     CS_REST_Subscribers
    cm.templates       CS_REST_Templates

    $ drush at_cm cm.general

    Class                 Method                Params
    ----------            ------------          ---------------------
    CS_REST_General       authorize_url         ($client_id, $redirect_uri, $scope, $state)
    CS_REST_General       get_clients           ()

    $ drush at_cm cm.general get_clients

    Array
    (
        [0] => stdClass Object
            (
                [ClientID] => ***
                [Name] => GO1
            )

    )

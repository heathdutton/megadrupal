Documentation
==============
- Guzzle documentation: http://guzzlephp.org/
- Guzzle service description: http://guzzlephp.org/guide/service/service_descriptions.html
- Guzzle Client instance building from description: http://guzzlephp.org/tour/building_services.html

Installation
==============
- Service module with REST server is required
- Service Guzzle module
- You will need Guzzle library, so you can use http://drupal.org/project/guzzle or provide library by yourself
- Create your endpoint for REST server and go to Guzzle tab
- Save service description into service.json file (optional)

Debugger UI
==============
You can test any operation of your endpoint on admin/config/services/guzzle_client.
Just give endpoint or service description json string and choose which operation you want to test.

Usage
==============
You can re-use DrupalServicesClient class to create GuzzleClient instance. There are two ways:

  1) From service.json file. Create client
      $client = \DrupalServices\DrupalServicesClient::factory(array('includes' => array($path_to_json . '/service.json'));

  2) From code. Using array of options
      $client = \DrupalServices\DrupalServicesClient::factory($array_of_service_description);

      Using object of options
        $client = \DrupalServices\DrupalServicesClient::factory(_services_guzzle_client_object_to_array_recursively($object_of_service_description));

and it's all, you have instance of GuzzleClient class that is ready to use with your particular endpoint.

For example you have node resources enabled in your endpoint. It has CRUD operations, lets find Retrieve operation
definition in our Guzzle service description:

  "NodeOperationRetrieve":{
    "httpMethod":"GET",
    "uri":"node/{nid}",
    "summary":"Retrieve a node",
    "parameters":{
      "nid":{
        "description":"The nid of the node to retrieve",
        "required":true,
        "location":"uri",
        "type":"integer"
      }
    }
  },

the main information you need to use any operation are
  - Guzzle name of operation "NodeOperationRetrieve"
  - array of parameters

so lets retrieve node with nid = 1

  $arguments = array('nid' => 1);
  $command = $client->getCommand('NodeOperationRetrieve', $arguments);
  $response = $client->execute($command);

and we get node array (not an object!). What can be easier?

To update node or any other operation you need just give name of another command (NodeOperationUpdate for example)
and provide necessary parameters (array('title' => 'New title', 'body' => array('und' => array(array('value' => 'Modified body')))

Some limitations:
- Some operation may need authorising and it depends on your permissions
- You get answers with arrays and have to provide params as array also
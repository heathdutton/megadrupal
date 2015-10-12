<?php

/**
 * @file
 * Fake requests for testing purposes.
 */

/**
 * Request calss for testing purpose.
 *
 * Returns some predefined data to be used in tests.
 */
class ServicesClientConnectionTestRequest extends ServicesClientConnectionRequest {

  /**
   * Implements ServicesClientConnectionRequest
   *
   * @param ServicesClientConnectionHttpRequest $request
   */
  public function call(ServicesClientConnectionHttpRequest &$request) {
    $result = new ServicesClientConnectionResponse();

    if ($request->action == 'retrieve' && $request->resource == 'index') {
      $result->response_code = 200;
      $result->raw_response = <<<JSON
[{"uuid":"3558f5cc-1aa7-47e9-8a31-49a0e85f9515","entity_id":"2","entity_type":"node","entity_bundle":"article","title":"Curabitur non turpis sem","keywords":"Curabitur non turpis sem Foo","status":"1","url":"http://sc/node/2","entity_created":"1341836634","entity_changed":"1341845695"},
 {"uuid":"6c8a792d-c4e0-447d-b235-5325c891826f","entity_id":"3","entity_type":"node","entity_bundle":"page","title":"Camur Cogo Elit Metuo Roto Volutpat","keywords":"Camur Cogo Elit Metuo Roto Volutpat","status":"1","url":"http://sc/node/3","entity_created":"1341920658","entity_changed":"1341920658"},
 {"uuid":"f27e1791-1642-40d6-ac42-666a85d1bf52","entity_id":"28","entity_type":"node","entity_bundle":"page","title":"Facilisis Os Probo","keywords":"Facilisis Os Probo","status":"1","url":"http://default/node/28","entity_created":"1342509766","entity_changed":"1342509766"},
 {"uuid":"395763e4-4632-4597-8007-7a211aaebb3a","entity_id":"29","entity_type":"node","entity_bundle":"article","title":"Inhibeo Uxor","keywords":"Inhibeo Uxor Foo","status":"1","url":"http://default/node/29","entity_created":"1342509766","entity_changed":"1342509766"},
 {"uuid":"34f08bd4-f1fb-4901-9800-bfceba7a36c9","entity_id":"30","entity_type":"node","entity_bundle":"article","title":"Molior Probo Secundum Typicus Verto","keywords":"Molior Probo Secundum Typicus Verto Foo","status":"1","url":"http://default/node/30","entity_created":"1342509766","entity_changed":"1342509766"},
 {"uuid":"52fd64f4-7fdc-4b9d-8909-9f88096e6ae2","entity_id":"31","entity_type":"node","entity_bundle":"page","title":"Ad Iustum","keywords":"Ad Iustum","status":"1","url":"http://default/node/31","entity_created":"1342509766","entity_changed":"1342509766"},
 {"uuid":"d1db4480-9be8-4769-8100-caca1e8265f6","entity_id":"32","entity_type":"node","entity_bundle":"article","title":"Camur Voco","keywords":"Camur Voco Foo","status":"1","url":"http://sc/node/32","entity_created":"1342509766","entity_changed":"1342530675"}
]
JSON;
    }

    return $result;
  }
}
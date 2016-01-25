<?php
/**
 * @file
 * Defines SearchApiDataseedConnection class which handles all the
 * server-to-server communications to the Dataseed Server specific for the
 * operations on the Dataseed index (dataset).
 *
 * Basically, it is used for:
 *  - creating/deleting Dataseed datasets
 *  - adding/removing Dataseed observations
 */

class SearchApiDataseedConnection extends DataseedConnection {

  /**
   * Response code returned by Dataseed APIs when a dataset creation request
   * raises no error.
   */
  const CREATE_DATASET_SUCCESS_RESPONSE_CODE = 303;


  /**
   * Creates a new Dataseed dataset.
   *
   * @param string $model
   *   the JSON dataset model (check the Dataseed documentation at
   *   http://dataseedapp.com/documentation)
   */
  public function createDataset($model) {
    $options  = array(
      'method' => 'POST',
      'data'   => $model,
    );
    $url      = $this->baseUrl . 'api/datasets/';
    $response = $this->makeHttpRequest($url, $options);

    return $this->checkResponse($response, self::CREATE_DATASET_SUCCESS_RESPONSE_CODE);

  }


  /**
   * Removes a Dataseed dataset.
   *
   * @param string $dataset_id
   *   the id of the dataset to be removed.
   *
   * @return bool
   *   TRUE in case of success
   * @throws SearchApiException
   *   if the Dataseed API is not successfully invoked.
   */
  public function removeDataset($dataset_id) {

    if (!$this->datasetExists($dataset_id)) {
      return TRUE;
    }

    $options = array(
      'method' => 'DELETE',
    );

    $url      = $this->baseUrl . "api/datasets/$dataset_id";
    $response = $this->makeHttpRequest($url, $options);

    return $this->checkResponse($response);
  }

  /**
   * Makes the POST request for adding Observations into Dataseed.
   *
   * @param array $observations
   *   the observations to be added
   * @param string $dataset_id
   *   the dataset id the observation must be added to
   *
   * @return bool
   *   TRUE in case of success
   * @throws SearchApiException
   *   if the Dataseed API is not successfully invoked.
   */
  public function addObservations($observations, $dataset_id) {

    $data               = new stdClass();
    $data->observations = array();

    foreach ($observations as $id => $item) {
      $o     = new stdClass();
      $o->id = $id;

      foreach ($item as $key => $field) {
        if ($key == 'search_api_language') {
          continue;
        }
        if ($field['type'] == 'date') {
          $o->{$key} = date('c', (int) $field['value']);
        }
        else {
          $o->{$key} = $field['value'];
        }
      }

      $data->observations[] = $o;
    }

    $json_data = drupal_json_encode($data);

    $options = array(
      'method' => 'PUT',
      'data'   => $json_data,
    );

    $url      = $this->baseUrl . "api/datasets/$dataset_id/observations/";
    $response = $this->makeHttpRequest($url, $options);

    return $this->checkResponse($response);

  }

  /**
   * Deletes all the observations of a dataset.
   *
   * @param string $dataset_id
   *   the id of the dataset the observations must be removed from
   *
   * @return bool
   *   TRUE in case of success
   * @throws SearchApiException
   *   if the Dataseed API is not successfully invoked.
   */
  public function deleteAllObservations($dataset_id) {
    $options = array(
      'method' => 'DELETE',
    );

    $url      = $this->baseUrl . "api/datasets/$dataset_id/observations/";
    $response = $this->makeHttpRequest($url, $options);
    return $this->checkResponse($response);
  }

  /**
   * Deletes individual observation.
   *
   * @param string $dataset_id
   *   the id of the dataset the observation must be removed from
   * @param string $observation_id
   *   the id of the observation to be removed
   *
   * @return bool
   *   TRUE in case of success
   * @throws SearchApiException
   *   if the Dataseed API is not successfully invoked.
   */
  public function deleteObservation($dataset_id, $observation_id) {
    $options = array(
      'method' => 'DELETE',
    );

    $url      = $this->baseUrl . "api/datasets/$dataset_id/observations/$observation_id";
    $response = $this->makeHttpRequest($url, $options);
    return $this->checkResponse($response);
  }
}

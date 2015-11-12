<?php

/**
 * This PHP file uses Drupal APIs. It cannot run without a Drupal installation.
 */

require_once 'Matrix.php';

// the maximum number of items the topSimilarity/topPrediction can return.
define('TOP_N_LIMIT', 500);

// the maximum number of records in one insert batch.
define('INSERT_LIMIT', 5000);



interface RecommenderInterface {

  /**
   * Initialize the recommender settings from $params. No complexity should be introduced here.
   * @param $params
   * @return null
   */
  public function initialize($params);

  /**
   * Do all computation here.
   * @return null
   */
  public function execute();

  /**
   * Save data to database, etc.
   * @return mixed any data that should be returned.
   */
  public function finalize();

}

/**
 * This is the classical collaborative filtering implementation.
 */
class CFRecommender implements RecommenderInterface {
  protected $structure;
  protected $timestamp;
  protected $userNum;
  protected $itemNum;
  protected $isBooleanRecommender;

  // large data
  protected $preferenceMatrix;
  protected $similarityMatrix;
  protected $predictionMatrix;
  protected $userVectors;
  protected $userMap;
  protected $itemMap;

  public function initialize($params) {
    $this->structure = recommender_prepare_data_structure($params['data structure']);
    $this->timestamp = time();
    $this->isBooleanRecommender = isset($this->structure['preference']['score type']) && ($this->structure['preference']['score type'] == 'boolean');
  }

  /**
   * Load matrix from the database into a matrix class in memory
   */
  protected function loadPreference() {
    $this->userNum = db_query("SELECT COUNT(DISTINCT {$this->structure['preference']['user field']}) FROM {$this->structure['preference']['name']}")->fetchField();
    $this->itemNum = db_query("SELECT COUNT(DISTINCT {$this->structure['preference']['item field']}) FROM {$this->structure['preference']['name']}")->fetchField();

    $fields = array(
      $this->structure['preference']['user field'],
      $this->structure['preference']['item field'],
      $this->structure['preference']['score field'],
    );
    if ($this->isBooleanRecommender) {
      unset($fields[2]);
    }

    $result = db_select($this->structure['preference']['name'], 'p')->fields('p', $fields)->execute();

    if ($this->userNum < 3 || $this->itemNum < 3 || $result->rowCount() < 10) {
      throw new LengthException('Cannot run recommender due to insufficient data.');
    }

    // create Sparse matrix, might fail if not enough memory.
    $matrix_type = $this->isBooleanRecommender ? 'RealMatrix' : 'SparseMatrix';
    $this->preferenceMatrix = Matrix::create($matrix_type, $this->userNum, $this->itemNum);

    $this->userMap = array();
    $this->itemMap = array();

    // build the matrix
    while ($row = $result->fetchAssoc()) {
      $user_id = $row[$this->structure['preference']['user field']];
      $item_id = $row[$this->structure['preference']['item field']];
      $score = $this->isBooleanRecommender ? 1 : $row[$this->structure['preference']['score field']];
      if (!array_key_exists($user_id, $this->userMap)) {
        $this->userMap[$user_id] = count($this->userMap);
      }
      if (!array_key_exists($item_id, $this->itemMap)) {
        $this->itemMap[$item_id] = count($this->itemMap);
      }
      $this->preferenceMatrix->set($this->userMap[$user_id], $this->itemMap[$item_id], $score);
    }
  }


  protected function computeSimilarity() {
    // regardless of the type of preferenceMatrix, similarityMatrix will be a "SparseMatrix".
    $this->similarityMatrix = Matrix::correlation($this->preferenceMatrix);
  }


  protected function computePrediction() {
    // we do the computation based on $this->preferenceMatrix loaded in memory
    $this->userVectors = $this->preferenceMatrix->row_vectors();
    // regardless of whether preferenceMatrix is a sparse matrix or not, predictionMatrix is always a sparseMatrix.
    $this->predictionMatrix = Matrix::create('SparseMatrix', $this->userNum, $this->itemNum);

    // calculate prediction for each user-item pair
    foreach ($this->userMap as $user_real_id => $user_matrix_index) {
      foreach ($this->itemMap as $item_real_id => $item_matrix_index) {
        // skip predictions on already existed preference ratings.
        if ((!$this->isBooleanRecommender && !is_nan($this->preferenceMatrix->get($user_matrix_index, $item_matrix_index))) || ($this->isBooleanRecommender && $this->preferenceMatrix->get($user_matrix_index, $item_matrix_index) != 0)) continue;

        // $user_matrix_index is the current user's matrix index to computing. $j is the "similar users"
        $numerator = 0;
        $denominator = 0;
        for ($j=0; $j< $this->userNum; $j++) {
          if ($j==$user_matrix_index) continue; // skip myself.
          if (is_nan($this->userVectors[$j]->get($item_matrix_index))) continue; // if no rating from j, skip.

          $similarity_value = $this->similarityMatrix->get($j, $user_matrix_index);
          if (is_nan($similarity_value)) continue; // skip if there is no similarity between $user_matrix_index and $j.

          $mean_j = $this->isBooleanRecommender ?
            $this->userVectors[$j]->mean(TRUE) :
            $this->userVectors[$j]->intersect_mean($this->userVectors[$user_matrix_index]);
          $normalized_j_score = $this->preferenceMatrix->get($j, $item_matrix_index) - $mean_j;

          $numerator += $normalized_j_score * $similarity_value;
          $denominator += abs($similarity_value);
        }
        if ($denominator != 0) {
          $prediction = $this->userVectors[$user_matrix_index]->mean(TRUE) + $numerator / $denominator;
          $this->predictionMatrix->set($user_matrix_index, $item_matrix_index, $prediction);
        }
      }
    }
  }


  protected function saveMatrix(&$map1, &$map2, &$result_matrix, $table_name, $source_field, $target_field, $score_field, $timestamp_field, $skip_self = FALSE) {
    $r_map1 = array_flip($map1);
    $r_map2 = array_flip($map2);
    $values = $result_matrix->raw_values();

    $insert = db_insert($table_name)->fields(array($source_field, $target_field, $score_field, $timestamp_field));

    foreach ($r_map1 as $v1 => $entity1) {
      foreach ($r_map2 as $v2 => $entity2) {
        if (!isset($values[$v1][$v2])) continue; // we might skip if it's undefined.
        if ($skip_self && $entity1 == $entity2) continue; // for similarity data, we want to skip self.
        $score = $values[$v1][$v2];
        if (!is_nan($score)) {
          $insert->values(array(
            $source_field => $entity1,
            $target_field => $entity2,
            $score_field => $score,
            $timestamp_field => $this->timestamp,
          ));
        } // end of if (score)
      } // end of for($v2)
    } // end of for($v1)

    $insert->execute();
  }


  public function execute() {
    // load preference data into database.
    $this->loadPreference();

    // compute user-similarities.
    $this->computeSimilarity();

    // compute predictions.
    $this->computePrediction();
  }


  public function finalize() {
    // save user similarities
    db_query("DELETE FROM {$this->structure['user similarity']['name']}");
    $this->saveMatrix($this->userMap, $this->userMap, $this->similarityMatrix, $this->structure['user similarity']['name'], $this->structure['user similarity']['user1 field'], $this->structure['user similarity']['user2 field'], $this->structure['user similarity']['score field'], $this->structure['user similarity']['timestamp field'], TRUE);

    // save predictions
    db_query("DELETE FROM {$this->structure['prediction']['name']}");
    $this->saveMatrix($this->userMap, $this->itemMap, $this->predictionMatrix, $this->structure['prediction']['name'], $this->structure['prediction']['user field'], $this->structure['prediction']['item field'], $this->structure['prediction']['score field'], $this->structure['prediction']['timestamp field']);

    // return other data
    return array(
      'num_user' => $this->userNum,
      'num_item' => $this->itemNum,
    );
  }
}


class UserBasedRecommender extends CFRecommender {
  // do nothing.
}


class ItemBasedRecommender extends CFRecommender {

  public function initialize($params) {
    // do a simple trick of inverse item/user fields.
    parent::initialize($params);
    $user_field = $this->structure['preference']['user field'];
    $item_field = $this->structure['preference']['item field'];
    $this->structure['preference']['user field'] = $item_field;
    $this->structure['preference']['item field'] = $user_field;
  }

  public function finalize() {
    // save item similarities
    db_query("DELETE FROM {$this->structure['item similarity']['name']}");
    $this->saveMatrix($this->userMap, $this->userMap, $this->similarityMatrix, $this->structure['item similarity']['name'], $this->structure['item similarity']['item1 field'], $this->structure['item similarity']['item2 field'], $this->structure['item similarity']['score field'], $this->structure['item similarity']['timestamp field'], TRUE);

    // save predictions
    db_query("DELETE FROM {$this->structure['prediction']['name']}");
    $this->saveMatrix($this->userMap, $this->itemMap, $this->predictionMatrix, $this->structure['prediction']['name'], $this->structure['prediction']['item field'], $this->structure['prediction']['user field'], $this->structure['prediction']['score field'], $this->structure['prediction']['timestamp field']);

    // note: this should be reverse.
    return array(
      'num_user' => $this->itemNum,
      'num_item' => $this->userNum,
    );
  }
}


class CFBooleanRecommender extends CFRecommender {
  public function initialize($params) {
    // do a simple trick of inverse item/user fields.
    parent::initialize($params);
    $this->structure['preference']['score type'] = 'boolean';
  }
}

class UserBasedBooleanRecommender extends CFBooleanRecommender {}

class ItemBasedBooleanRecommender extends ItemBasedRecommender {
  public function initialize($params) {
    // do a simple trick of inverse item/user fields.
    parent::initialize($params);
    $this->structure['preference']['score type'] = 'boolean';
  }
}
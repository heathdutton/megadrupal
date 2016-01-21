<?php

/**
 * @file
 * Classes for handling tree nodes using a specific matrix encoding.
 */

require_once 'BigNum.class.php';

class MatrixTree {
  public $nv;
  public $dv;
  public $snv;
  public $sdv;
  public $decimals;

  /**
   * Pretty-print this matrix.
   *
   * @param string $title
   *   Title of this matrix.
   */
  public function dumpMe($title) {
    $org_title = isset($this->title) ? $this->title : NULL;
    $this->title = $title;
    self::printMatrices(array($this));
    if (isset($org_title)) {
      $this->title = $org_title;
    }
    else {
      unset($this->title);
    }
  }

  /**
   * Constructor.
   *
   * Create a 2x2 matrix.
   *
   * @param string $nv
   *   Numerator.
   * @param string $dv
   *   Denominator.
   * @param string $snv
   *   Sibling numerator.
   * @param string $sdv
   *   Sibling denominator.
   */
  public function __construct($nv, $dv, $snv, $sdv) {
    $this->nv = (string) $nv;
    $this->dv = (string) $dv;
    $this->snv = (string) $snv;
    $this->sdv = (string) $sdv;
    $this->decimals = variable_get('taxonomy_nested_intervals_decimals', TAXONOMY_NESTED_INTERVALS_DECIMALS);
  }

  /**
   * Get root matrix.
   * @return MatrixTree
   *   Root matrix.
   */
  public static function getRoot() {
    return new self(1, 0, 0, 1);
  }

  /**
   * Create matrix from associative array.
   *
   * @param array $data
   *   Array containing matrix data.
   *
   * @return MatrixTree
   *   New matrix.
   */
  public static function createFromAssoc(array $data) {
    return new self(
      $data['nv'],
      $data['dv'],
      $data['snv'],
      $data['sdv']
    );
  }

  public static function getLeafMatrix($sibling = 1) {
    return new self($sibling, 1, 1, 0);
  }

  public static function printMatrices($matrices, $result = NULL, $titles = NULL, $title = NULL) {
    print self::formatMatrices($matrices, $result, $titles, $title);
  }

  public static function formatMatrices($matrices, $result = NULL, $titles = NULL, $title = NULL) {
    if ($title) {
      print "$title\n";
    }
    $rows = array();
    foreach ($matrices as $matrix) {
      $rows[-1][] = sprintf(" %7s ", empty($titles) ? (empty($matrix->title) ? '' : $matrix->title) : array_shift($titles));
      $rows[0][] = '+---+---+';
      $rows[1][] = sprintf("|%3s|%3s|", BigNum::floor($matrix->nv), BigNum::floor($matrix->snv));
      $rows[2][] = sprintf("|%3s|%3s|", BigNum::floor($matrix->dv), BigNum::floor($matrix->sdv));
      $rows[3][] = '+---+---+';
    }
    if ($result) {
      $rows[-1][] = '         ';
      $rows[0][] = '         ';
      $rows[1][] = '   ===   ';
      $rows[2][] = '   ===   ';
      $rows[3][] = '         ';
      $rows[-1][] = sprintf(" %7s ", empty($titles) ? (empty($result->title) ? '' : $result->title) : array_shift($titles));
      $rows[0][] = '+---+---+';
      $rows[1][] = sprintf("|%3s|%3s|", $result->nv, $result->snv);
      $rows[2][] = sprintf("|%3s|%3s|", $result->dv, $result->sdv);
      $rows[3][] = '+---+---+';
    }
    $output = '';
    foreach ($rows as $row) {
      $output .= implode('  ', $row) . "\n";
    }
    return $output;
  }

  /**
   * Convert an enumerated path into a set of matrices.
   */
  public static function pathToMatrices($path) {
    $matrices = array();

    if ($path != '') {
      $parts = explode('.', $path);
      foreach ($parts as $part) {
        $matrices[] = self::getLeafMatrix($part);
      }
    }
    if (empty($matrices)) {
      $matrices[] = self::getRoot();
    }
    return $matrices;
  }

  /**
   * Convert an enumerated path into a matrix.
   */
  public static function pathToMatrix($path) {
    return self::product(self::pathToMatrices($path));
  }

  /**
   * Multiply multiple matrices.
   */
  public static function product($matrices) {
    $matrix = array_shift($matrices);
    if (!$matrices) {
      return $matrix;
    }
    foreach ($matrices as $m) {
      $matrix = $matrix->multiply($m);
    }
    return $matrix;
  }

  public function getDeterminant() {
    return BigNum::sub(BigNum::mul($this->nv, $this->sdv), BigNum::mul($this->dv, $this->snv));
  }

  /**
   * Get an absolute value of the matrix.
   */
  public function abs() {
    $class = get_class($this);
    return new $class(
      BigNum::cmp($this->nv, '0') === -1 ? BigNum::mul($this->nv, -1) : $this->nv,
      BigNum::cmp($this->dv, '0') === -1 ? BigNum::mul($this->dv, -1) : $this->dv,
      BigNum::cmp($this->snv, '0') === -1 ? BigNum::mul($this->snv, -1) : $this->snv,
      BigNum::cmp($this->sdv, '0') === -1 ? BigNum::mul($this->sdv, -1) : $this->sdv
    );
  }

  /**
   * Transpose.
   */
  public function transpose() {
    $class = get_class($this);
    return new $class(
      $this->nv,
      $this->snv,
      $this->dv,
      $this->sdv
    );
  }

  /**
   * Multiply 2 matrices.
   */
  public function multiply($matrix) {
    $class = get_class($this);
    return new $class(
      BigNum::add(BigNum::mul($this->nv, $matrix->nv), BigNum::mul($this->snv, $matrix->dv)),
      BigNum::add(BigNum::mul($this->dv, $matrix->nv), BigNum::mul($this->sdv, $matrix->dv)),
      BigNum::add(BigNum::mul($this->nv, $matrix->snv), BigNum::mul($this->snv, $matrix->sdv)),
      BigNum::add(BigNum::mul($this->dv, $matrix->snv), BigNum::mul($this->sdv, $matrix->sdv))
    );
  }

  /**
   * Multiply matrix by a scalar.
   */
  public function scalarMultiply($scalar) {
    $class = get_class($this);
    return new $class(
      BigNum::mul($this->nv, $scalar),
      BigNum::mul($this->dv, $scalar),
      BigNum::mul($this->snv, $scalar),
      BigNum::mul($this->sdv, $scalar)
    );
  }

  /**
   * Return the inverse of the matrix.
   *
   * The determinant is always 1
   */
  public function getInverse() {
    return $this->getAdjugate()->scalarMultiply(BigNum::div(1, $this->getDeterminant()));
  }

  public function getAdjugate() {
    $class = get_class($this);
    return new $class(
      $this->sdv,
      -$this->dv,
      -$this->snv,
      $this->nv
    );
  }

  public function getLeaf() {
    return $this->getLeafMatrix($this->getEnumeratedSibling());
  }

  public function getMatrices() {
    $parents = array();
    $parent = $this;
    do {
      $parents[] = $parent->getLeaf();
      $parent = $parent->getParent();
    } while ($parent);
    return array_reverse($parents);
  }

  public function getPath() {
    $parent = $this;
    do {
      $np[] = $parent->getEnumeratedSibling();
      $parent = $parent->getParent();
    } while ($parent);
    return implode('.', array_reverse($np));
  }

  public function getDepth() {
    $depth = -1;
    $parent = $this;
    do {
      $depth++;
      $parent = $parent->getParent();
    } while ($parent);
    return $depth;
  }

  public function getPreviousSibling() {
    $class = get_class($this);
    return new $class($this->nv - $this->snv, $this->dv - $this->sdv, $this->snv, $this->sdv);
  }

  public function getNextSibling() {
    $class = get_class($this);
    return new $class($this->nv + $this->snv, $this->dv + $this->sdv, $this->snv, $this->sdv);
  }

  public function getSafeParent() {
    $parent = $this->getParent();
    return $parent ? $parent : $this->getRoot();
  }

  public function getParent() {
    if (BigNum::cmp($this->dv, 0) && BigNum::cmp($this->sdv, 0)) {
      return $this->multiply($this->getLeaf()->getInverse());
    }
    return;
  }

  public function getEnumeratedSibling() {
    $result = BigNum::cmp($this->snv, 1) === 0 && BigNum::cmp($this->sdv, 0) !== 0 ? $this->dv : BigNum::div($this->nv, $this->snv);
    return intval($result);
  }

  public function getLeft($factor = 1) {
    $lft = BigNum::round(BigNum::div(BigNum::mul($this->nv, $factor), $this->dv, $this->decimals + 1), $this->decimals);
    $rgt = BigNum::round(BigNum::div(BigNum::mul($this->nv + $this->snv, $factor), $this->dv + $this->sdv, $this->decimals + 1), $this->decimals);
    return BigNum::cmp($lft, $rgt, $this->decimals) === -1 ? ($this->getEnumeratedSibling() == 1 ? BigNum::add($lft, '0.' . str_repeat('0', $this->decimals - 1) . '1', $this->decimals) : $lft) : $rgt;
  }

  public function getRight($factor = 1) {
    $lft = BigNum::round(BigNum::div(BigNum::mul($this->nv, $factor), $this->dv, $this->decimals + 1), $this->decimals);
    $rgt = BigNum::round(BigNum::div(BigNum::mul($this->nv + $this->snv, $factor), $this->dv + $this->sdv, $this->decimals + 1), $this->decimals);
    return BigNum::cmp($lft, $rgt, $this->decimals) === 1 ? $lft : $rgt;
  }

  public function generateLeaf($sibling = 1) {
    $class = get_class($this);
    return $this->multiply($class::getLeafMatrix($sibling));
    $result = $class::product(array($this, $class::getLeafMatrix($sibling)));
    $class::printMatrices(array($this, $class::getLeafMatrix($sibling), $result));
    return $result;
  }

  public function generateSibling($sibling) {
    $parent = $this->getParent();
    return $parent ? $this->getParent()->generateLeaf($sibling) : self::pathToMatrix("$sibling");
  }

  public function relocate($dst, $delta_sibling = NULL) {
    $delta_sibling = isset($delta_sibling) ? $delta_sibling : $dst->getEnumeratedSibling() - $this->getEnumeratedSibling();
    return self::product(array(
      $dst->getSafeParent(), // New parent (attach new parent).
      new self(1, 0, $delta_sibling, 1), // Sibling bump.
      $this->getSafeParent()->getInverse() // Inverse of old parent (detach old parent).
    ));
  }

  public function getScopeForBump($matrix, $factor) {
    $parent = $matrix->getParent();
    if ($matrix->getPolarity() == -1) {
      // Use corrected scope when compensating for polarity.
      $lft = $parent->getLeft($factor);
      $lft = BigNum::add($lft, '0.000000000000000000000000000001', variable_get('taxonomy_nested_intervals_decimals', TAXONOMY_NESTED_INTERVALS_DECIMALS));
      $rgt = $matrix->getRight($factor);
    }
    else {
      $lft = $matrix->getLeft($factor);
      $rgt = $parent ? $parent->getRight($factor) : 0;
    }
    return array($lft, $rgt);
  }

  static public function getPolarityByDistance($distance) {
    return $distance % 2 == 0 ? 1 : -1;
  }

  public function getPolarity() {
    $class = get_class($this);
    return $class::getPolarityByDistance($this->getDepth());
  }
}

class MatrixTreeTropashko extends MatrixTree {
}

class MatrixTreeHazel extends MatrixTree {

  public static function getRoot() {
    return new self(0, 1, 1, 0);
  }

  public static function createFromAssoc(array $data) {
    return new self(
      $data['nv'],
      $data['dv'],
      $data['snv'],
      $data['sdv']
    );
  }

  public static function getLeafMatrix($sibling = 1) {
    return new self(1, $sibling, 1, BigNum::add($sibling, 1));
  }

  public static function pathToMatrices($path) {
    $matrices = array();
    $matrices[] = self::getRoot();

    if ($path != '') {
      $parts = explode('.', $path);
      foreach ($parts as $part) {
        $matrices[] = self::getLeafMatrix($part);
      }
    }
    return $matrices;
  }

  public static function pathToMatrix($path) {
    return self::product(self::pathToMatrices($path));
  }

  /**
   * Return the inverse of the matrix.
   *
   * The determinant is always -1
   */
  public function getInverse() {
    $class = get_class($this);
    return new $class(
      -$this->sdv,
      $this->dv,
      $this->snv,
      -$this->nv
    );
  }

  public function getMatrices() {
    $parents = array();
    $parent = $this;
    do {
      $parents[] = $parent->getLeaf();
      $parent = $parent->getParent();
    } while ($parent);
    $parents[] = self::getRoot();
    return array_reverse($parents);
  }

  public function getPath() {
    return parent::getPath();
  }

  public function getDepth() {
    return parent::getDepth();
  }

  public function getPreviousSibling() {
    $class = get_class($this);
    return new $class($this->nv * 2 - $this->snv, $this->dv * 2 - $this->snv, $this->nv, $this->dv);
  }

  public function getNextSibling() {
    $class = get_class($this);
    return new $class($this->snv, $this->sdv, $this->snv * 2 - $this->nv, $this->sdv * 2 - $this->dv);
  }

  public function getParent() {
    if ($result = parent::getParent()) {
      if (intval($result->nv) == 0) {
        return;
      }
    }
    return $result;
  }

  public function getEnumeratedSibling() {
    return intval(BigNum::div($this->nv, BigNum::sub($this->snv, $this->nv)));
  }

  public function getLeft($factor = 1) {
    return BigNum::round(BigNum::div(BigNum::mul($this->nv, $factor), $this->dv, $this->decimals + 1), $this->decimals);
    return BigNum::div(BigNum::mul($this->nv, $factor), $this->dv, $this->decimals);
  }

  public function getRight($factor = 1) {
    return BigNum::round(BigNum::div(BigNum::mul($this->snv, $factor), $this->sdv, $this->decimals + 1), $this->decimals);
  }

  public function generateSibling($sibling) {
    $parent = $this->getParent();
    return $parent ? $this->getParent()->generateLeaf($sibling) : self::pathToMatrix("$sibling");
  }

  public function relocate($dst, $delta_sibling = NULL) {
    $delta_sibling = isset($delta_sibling) ? $delta_sibling : $dst->getEnumeratedSibling() - $this->getEnumeratedSibling();
    return self::product(array(
      $dst->getSafeParent(), // New parent (attach new parent).
      new self(1, $delta_sibling, 0, 1), // Sibling bump.
      $this->getSafeParent()->getInverse() // Inverse of old parent (detach old parent).
    ));
  }

  public function getScopeForBump($matrix, $factor) {
    // Calculate scope of sibling bump = [our left;parent right[.
    $lft = $matrix->getLeft($factor);
    $parent = $matrix->getParent();
    $rgt = $parent ? $parent->getRight($factor) : 0;
    return array($lft, $rgt);
  }

  static public function getPolarityByDistance($distance) {
    return 1;
  }

  public function getPolarity() {
    return 1;
  }
}

<?php
/*************************************************************************
adds visitor to audience.db when they click the join button
==========================================================================
adapted from www.thomas.vanhoutte.be/miniblog/visitor-counter/
*************************************************************************/

$dbfile = "audience.db"; // path to file storing number of visitors

if(!file_exists($dbfile)) {
    die("Error: Data file " . $dbfile . " NOT FOUND!");
}
if(!is_writable($dbfile)) {
    die("Error: Data file " . $dbfile . " is NOT writable! Please CHMOD it to 666!");
}

function AddVisitor() {
    global $dbfile;
    $cur_ip = getIP();
    $cur_time = time();
    $dbary_new = array();
 
    $dbary = unserialize(file_get_contents($dbfile));
    if(is_array($dbary)) {
        while(list($user_ip, $user_time) = each($dbary)) {
            // unlike visitors.php, doesn't check whether they've been removed. 
            //potentially problematic but trying not to be redundant.
            $dbary_new[$user_ip] = $user_time; // re-adding them into the new array
        }
    }
    $dbary_new[$cur_ip] = $cur_time; // add record for current user
 
    $fp = fopen($dbfile, "w");
    fputs($fp, serialize($dbary_new));
    fclose($fp);
 
 // this will become visitornum
    $out = sprintf("%03d", count($dbary_new)); // format the result to display 3 digits with leading 0's
    return $out;
}

function getIP() {
    if(isset($_SERVER['HTTP_X_FORWARDED_FOR'])) $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
    elseif(isset($_SERVER['REMOTE_ADDR'])) $ip = $_SERVER['REMOTE_ADDR'];
    else $ip = "0";
    return $ip;
}

$audiencenum = AddVisitor();

$fp = fopen('visitornum.txt', 'w');
fwrite($fp, $audiencenum);
fclose($fp);
?>

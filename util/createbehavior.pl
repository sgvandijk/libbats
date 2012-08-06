#!/usr/bin/perl -w

$header = <<EOF
#ifndef _%NAMESPACE%_%NAME%_HH_
#define _%NAMESPACE%_%NAME%_HH_

#include <libbats/Behavior/behavior.hh>

namespace %namespace%
{
  class %Name% : public bats::Behavior
  {

    virtual bats::rf<bats::Goal> generateGoal(unsigned step, unsigned slot);
    virtual bats::rf<bats::State> getCurrentState();
    virtual bats::ConfidenceInterval getCapability(bats::rf<bats::State> s, bats::rf<bats::Goal> g);
  
  public:
    %Name%(std::string const &id, std::string const &playerClass);
  };
};

#endif
EOF
;

######################################################

$iheader = <<EOF
#include "%name%.hh"

#include <libbats/WorldModel/worldmodel.hh>
#include <libbats/AgentModel/agentmodel.hh>

using namespace bats;
using namespace std;
using namespace Eigen;
using namespace %namespace%;
EOF
;

######################################################

$constructor = <<EOF
#include "%name%.ih"

%Name%::%Name%(string const &id, string const &playerClass)
  : Behavior("%Name%", id, playerClass)
{
  d_tree = new AST::Node(sequenceType);
  d_tree->addChild(new AST::Node(andType));
  d_tree->getChild(0)->addChild(new AST::Node(orType));
}
EOF
;

######################################################

$generategoal = <<EOF
#include "%name%.ih"

rf<Goal> %Name%::generateGoal(unsigned step, unsigned slot)
{
  rf<Goal> goal = new Goal();
  rf<OrNode> dis = goal->addDisjunct();
  rf<AndNode> con = dis->addConjunct();
  
  // con->addVar("Var", 0, 1);
  
  return goal;
}
EOF
;

######################################################

$getcurrentstate = <<EOF
#include "%name%.ih"

rf<State> %Name%::getCurrentState()
{
  rf<State> state = new State();
  rf<OrNode> dis = state->addDisjunct();
  rf<AndNode> con = dis->addConjunct();

  // con->addVar("Var", 0, 1);
  return state;
}
EOF
;

######################################################

$getcapability = <<EOF
#include "%name%.ih"

ConfidenceInterval %Name%::getCapability(rf<State> s, rf<Goal> g)
{
  return capable(true);
}
EOF
;

$dir = "Behavior";
$namespace = "bats";
$name = "";

foreach $i (0..$#ARGV) {
  if ($ARGV[$i] eq "-d") {
    $dir = $ARGV[++$i];
  } elsif ($ARGV[$i] eq "-n") {
    $namespace = $ARGV[++$i];
  } else {
    $name = $ARGV[$i];
  }
}

print "Usage: createbehavior.pl [-d <directory>] [-n <namespace>] <behavior>\n" and exit if ($name eq "");

$nameLower = lc $name;
$nameUpper = uc $name;

$namespaceUpper = uc $namespace;

$header =~ s/\%Name\%/$name/g;
$header =~ s/\%NAME\%/$nameUpper/g;
$header =~ s/\%NAMESPACE\%/$namespaceUpper/g;
$header =~ s/\%namespace\%/$namespace/g;

$iheader =~ s/\%name\%/$nameLower/g;
$iheader =~ s/\%namespace\%/$namespace/g;

$constructor =~ s/\%name\%/$nameLower/g;
$constructor =~ s/\%Name\%/$name/g;

$generategoal =~ s/\%name\%/$nameLower/g;
$generategoal =~ s/\%Name\%/$name/g;

$getcurrentstate =~ s/\%name\%/$nameLower/g;
$getcurrentstate =~ s/\%Name\%/$name/g;

$getcapability =~ s/\%name\%/$nameLower/g;
$getcapability =~ s/\%Name\%/$name/g;

$dir = "$dir/$name";

mkdir $dir or die "Couldn't create $dir!\n";

open(HEADER, ">$dir/$nameLower.hh");
print HEADER $header;
close(HEADER);

open(IHEADER, ">$dir/$nameLower.ih");
print IHEADER $iheader;
close(IHEADER);

open(CONSTRUCTOR, ">$dir/$name.cc");
print CONSTRUCTOR $constructor;
close(CONSTRUCTOR);

open(GENERATEGOAL, ">$dir/generateGoal.cc");
print GENERATEGOAL $generategoal;
close(GENERATEGOAL);

open(GETCURRENTSTATE, ">$dir/getCurrentState.cc");
print GETCURRENTSTATE $getcurrentstate;
close(GETCURRENTSTATE);

open(GETCAPABILITY, ">$dir/getCapability.cc");
print GETCAPABILITY $getcapability;
close(GETCAPABILITY);

print "Behavior $name created!\n";


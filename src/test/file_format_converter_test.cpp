/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2018, The Souffle Developers. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file souffle_profile_test.cpp
 *
 * Tests for souffle-profile and profilerlib.
 *
 ***********************************************************************/

#include "FileFormatConverter.h"
#include "test.h"

#include <sstream>

namespace souffle {

namespace test {

TEST(FileFormatConverter, fromLogToCsv) {
    const auto logFilePath = "/tmp/souffle_file_format_converter_test.log";
    const auto logFileContent =
            "@start-debug\n"
            "@t-nonrecursive-rule;base;/mnt/src/test/foo.o/tc/tc.dl "
            "[29:1-29:15];base(\\\"a\\\",\\\"b\\\").;2.234e-06\n"
            "@n-nonrecursive-rule;base;/mnt/src/test/foo.o/tc/tc.dl "
            "[29:1-29:15];base(\\\"a\\\",\\\"b\\\").;4\n"
            "@t-nonrecursive-rule;base;/mnt/src/test/foo.o/tc/tc.dl "
            "[30:1-30:15];base(\\\"b\\\",\\\"c\\\").;1.888e-06\n"
            "@n-nonrecursive-rule;base;/mnt/src/test/foo.o/tc/tc.dl "
            "[30:1-30:15];base(\\\"b\\\",\\\"c\\\").;5\n"
            "@t-nonrecursive-rule;base;/mnt/src/test/foo.o/tc/tc.dl "
            "[31:1-31:15];base(\\\"c\\\",\\\"d\\\").;1.228e-06\n"
            "@n-nonrecursive-rule;base;/mnt/src/test/foo.o/tc/tc.dl "
            "[31:1-31:15];base(\\\"c\\\",\\\"d\\\").;6\n"
            "@t-nonrecursive-relation;base;/mnt/src/test/foo.o/tc/tc.dl [10:7-0:0];7.2986e-05\n"
            "@n-nonrecursive-relation;base;/mnt/src/test/foo.o/tc/tc.dl [10:7-0:0];6\n"
            "@t-nonrecursive-rule;tc;/mnt/src/test/foo.o/tc/tc.dl [25:1-25:22];tc(X,Y) :- \\n   "
            "base(X,Y).;7.5669e-05\n"
            "@n-nonrecursive-rule;tc;/mnt/src/test/foo.o/tc/tc.dl [25:1-25:22];tc(X,Y) :- \\n   "
            "base(X,Y).;6\n"
            "@t-nonrecursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];0.000134419\n"
            "@n-nonrecursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];6\n"
            "@t-recursive-rule;tc;0;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;9.725e-06\n"
            "@n-recursive-rule;tc;0;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;0\n"
            "@t-recursive-rule;tc;1;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;8.908e-06\n"
            "@n-recursive-rule;tc;1;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;5\n"
            "@t-recursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];2.6422e-05\n"
            "@n-recursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];5\n"
            "@c-recursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];1.9082e-05\n"
            "@t-recursive-rule;tc;0;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;4.833e-06\n"
            "@n-recursive-rule;tc;0;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;4\n"
            "@t-recursive-rule;tc;1;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;3.2542e-05\n"
            "@n-recursive-rule;tc;1;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;7\n"
            "@t-recursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];0.000107862\n"
            "@n-recursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];7\n"
            "@c-recursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];5.583e-06\n"
            "@t-recursive-rule;tc;0;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;1.0213e-05\n"
            "@n-recursive-rule;tc;0;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;3\n"
            "@t-recursive-rule;tc;1;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;7.169e-06\n"
            "@n-recursive-rule;tc;1;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;3\n"
            "@t-recursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];2.3166e-05\n"
            "@n-recursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];3\n"
            "@c-recursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];1.873e-06\n"
            "@t-recursive-rule;tc;0;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;1.564e-06\n"
            "@n-recursive-rule;tc;0;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;0\n"
            "@t-recursive-rule;tc;1;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;5.099e-06\n"
            "@n-recursive-rule;tc;1;/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28];tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).;0\n"
            "@t-recursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];1.0396e-05\n"
            "@n-recursive-relation;tc;/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0];0\n"
            "@t-nonrecursive-rule;tcl;/mnt/src/test/foo.o/tc/tc.dl [17:1-17:24];tcl(X,Y) :- \\n   "
            "base(X,Y).;2.2973e-05\n"
            "@n-nonrecursive-rule;tcl;/mnt/src/test/foo.o/tc/tc.dl [17:1-17:24];tcl(X,Y) :- \\n   "
            "base(X,Y).;6\n"
            "@t-nonrecursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];3.8938e-05\n"
            "@n-nonrecursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];6\n"
            "@t-recursive-rule;tcl;0;/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33];tcl(X,Y) :- \\n   "
            "tcl(X,Z),\\n   base(Z,Y).;1.771e-05\n"
            "@n-recursive-rule;tcl;0;/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33];tcl(X,Y) :- \\n   "
            "tcl(X,Z),\\n   base(Z,Y).;5\n"
            "@t-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];3.4923e-05\n"
            "@n-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];5\n"
            "@c-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];6.135e-06\n"
            "@t-recursive-rule;tcl;0;/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33];tcl(X,Y) :- \\n   "
            "tcl(X,Z),\\n   base(Z,Y).;8.973e-06\n"
            "@n-recursive-rule;tcl;0;/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33];tcl(X,Y) :- \\n   "
            "tcl(X,Z),\\n   base(Z,Y).;4\n"
            "@t-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];1.8546e-05\n"
            "@n-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];4\n"
            "@c-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];3.684e-06\n"
            "@t-recursive-rule;tcl;0;/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33];tcl(X,Y) :- \\n   "
            "tcl(X,Z),\\n   base(Z,Y).;6.006e-06\n"
            "@n-recursive-rule;tcl;0;/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33];tcl(X,Y) :- \\n   "
            "tcl(X,Z),\\n   base(Z,Y).;3\n"
            "@t-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];1.369e-05\n"
            "@n-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];3\n"
            "@c-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];3.535e-06\n"
            "@t-recursive-rule;tcl;0;/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33];tcl(X,Y) :- \\n   "
            "tcl(X,Z),\\n   base(Z,Y).;4.448e-06\n"
            "@n-recursive-rule;tcl;0;/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33];tcl(X,Y) :- \\n   "
            "tcl(X,Z),\\n   base(Z,Y).;2\n"
            "@t-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];8.376e-06\n"
            "@n-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];2\n"
            "@c-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];2.41e-06\n"
            "@t-recursive-rule;tcl;0;/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33];tcl(X,Y) :- \\n   "
            "tcl(X,Z),\\n   base(Z,Y).;3.865e-06\n"
            "@n-recursive-rule;tcl;0;/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33];tcl(X,Y) :- \\n   "
            "tcl(X,Z),\\n   base(Z,Y).;1\n"
            "@t-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];9.393e-06\n"
            "@n-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];1\n"
            "@c-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];3.9134e-05\n"
            "@t-recursive-rule;tcl;0;/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33];tcl(X,Y) :- \\n   "
            "tcl(X,Z),\\n   base(Z,Y).;3.161e-06\n"
            "@n-recursive-rule;tcl;0;/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33];tcl(X,Y) :- \\n   "
            "tcl(X,Z),\\n   base(Z,Y).;0\n"
            "@t-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];8.595e-06\n"
            "@n-recursive-relation;tcl;/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0];0\n"
            "@t-nonrecursive-rule;tcr;/mnt/src/test/foo.o/tc/tc.dl [21:1-21:23];tcr(X,Y) :- \\n   "
            "base(X,Y).;1.2961e-05\n"
            "@n-nonrecursive-rule;tcr;/mnt/src/test/foo.o/tc/tc.dl [21:1-21:23];tcr(X,Y) :- \\n   "
            "base(X,Y).;6\n"
            "@t-nonrecursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];2.9421e-05\n"
            "@n-nonrecursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];6\n"
            "@t-recursive-rule;tcr;0;/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33];tcr(X,Y) :- \\n   "
            "base(X,Z),\\n   tcr(Z,Y).;1.0624e-05\n"
            "@n-recursive-rule;tcr;0;/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33];tcr(X,Y) :- \\n   "
            "base(X,Z),\\n   tcr(Z,Y).;5\n"
            "@t-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];1.6128e-05\n"
            "@n-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];5\n"
            "@c-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];3.425e-06\n"
            "@t-recursive-rule;tcr;0;/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33];tcr(X,Y) :- \\n   "
            "base(X,Z),\\n   tcr(Z,Y).;1.1101e-05\n"
            "@n-recursive-rule;tcr;0;/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33];tcr(X,Y) :- \\n   "
            "base(X,Z),\\n   tcr(Z,Y).;4\n"
            "@t-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];1.5448e-05\n"
            "@n-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];4\n"
            "@c-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];1.0178e-05\n"
            "@t-recursive-rule;tcr;0;/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33];tcr(X,Y) :- \\n   "
            "base(X,Z),\\n   tcr(Z,Y).;9.23e-06\n"
            "@n-recursive-rule;tcr;0;/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33];tcr(X,Y) :- \\n   "
            "base(X,Z),\\n   tcr(Z,Y).;3\n"
            "@t-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];1.4339e-05\n"
            "@n-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];3\n"
            "@c-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];2.692e-06\n"
            "@t-recursive-rule;tcr;0;/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33];tcr(X,Y) :- \\n   "
            "base(X,Z),\\n   tcr(Z,Y).;9.306e-06\n"
            "@n-recursive-rule;tcr;0;/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33];tcr(X,Y) :- \\n   "
            "base(X,Z),\\n   tcr(Z,Y).;2\n"
            "@t-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];1.5315e-05\n"
            "@n-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];2\n"
            "@c-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];3.329e-06\n"
            "@t-recursive-rule;tcr;0;/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33];tcr(X,Y) :- \\n   "
            "base(X,Z),\\n   tcr(Z,Y).;4.252e-06\n"
            "@n-recursive-rule;tcr;0;/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33];tcr(X,Y) :- \\n   "
            "base(X,Z),\\n   tcr(Z,Y).;1\n"
            "@t-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];7.3e-06\n"
            "@n-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];1\n"
            "@c-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];1.449e-06\n"
            "@t-recursive-rule;tcr;0;/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33];tcr(X,Y) :- \\n   "
            "base(X,Z),\\n   tcr(Z,Y).;1.892e-06\n"
            "@n-recursive-rule;tcr;0;/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33];tcr(X,Y) :- \\n   "
            "base(X,Z),\\n   tcr(Z,Y).;0\n"
            "@t-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];4.117e-06\n"
            "@n-recursive-relation;tcr;/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0];0\n"
            "@runtime;0.0118777";
    const auto csvFilePath = "/tmp/souffle_file_format_converter_test.csv";
    const auto expectedCsvFileContent =
            "'@','copy-time','relation','rule','src-locator','time','total-time','tuples','version'\n"
            "'@start-debug','','','','','','','',''\n"
            "'@t-nonrecursive-rule','','base','base(\\\"a\\\",\\\"b\\\").','/mnt/src/test/foo.o/tc/tc.dl "
            "[29:1-29:15]','2.234e-06','','',''\n"
            "'@n-nonrecursive-rule','','base','base(\\\"a\\\",\\\"b\\\").','/mnt/src/test/foo.o/tc/tc.dl "
            "[29:1-29:15]','','','4',''\n"
            "'@t-nonrecursive-rule','','base','base(\\\"b\\\",\\\"c\\\").','/mnt/src/test/foo.o/tc/tc.dl "
            "[30:1-30:15]','1.888e-06','','',''\n"
            "'@n-nonrecursive-rule','','base','base(\\\"b\\\",\\\"c\\\").','/mnt/src/test/foo.o/tc/tc.dl "
            "[30:1-30:15]','','','5',''\n"
            "'@t-nonrecursive-rule','','base','base(\\\"c\\\",\\\"d\\\").','/mnt/src/test/foo.o/tc/tc.dl "
            "[31:1-31:15]','1.228e-06','','',''\n"
            "'@n-nonrecursive-rule','','base','base(\\\"c\\\",\\\"d\\\").','/mnt/src/test/foo.o/tc/tc.dl "
            "[31:1-31:15]','','','6',''\n"
            "'@t-nonrecursive-relation','','base','','/mnt/src/test/foo.o/tc/tc.dl "
            "[10:7-0:0]','7.2986e-05','','',''\n"
            "'@n-nonrecursive-relation','','base','','/mnt/src/test/foo.o/tc/tc.dl [10:7-0:0]','','','6',''\n"
            "'@t-nonrecursive-rule','','tc','tc(X,Y) :- \\n   base(X,Y).','/mnt/src/test/foo.o/tc/tc.dl "
            "[25:1-25:22]','7.5669e-05','','',''\n"
            "'@n-nonrecursive-rule','','tc','tc(X,Y) :- \\n   base(X,Y).','/mnt/src/test/foo.o/tc/tc.dl "
            "[25:1-25:22]','','','6',''\n"
            "'@t-nonrecursive-relation','','tc','','/mnt/src/test/foo.o/tc/tc.dl "
            "[13:7-0:0]','0.000134419','','',''\n"
            "'@n-nonrecursive-relation','','tc','','/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0]','','','6',''\n"
            "'@t-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','9.725e-06','','','0'\n"
            "'@n-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','','','0','0'\n"
            "'@t-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','8.908e-06','','','1'\n"
            "'@n-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','','','5','1'\n"
            "'@t-recursive-relation','','tc','','/mnt/src/test/foo.o/tc/tc.dl "
            "[13:7-0:0]','2.6422e-05','','',''\n"
            "'@n-recursive-relation','','tc','','/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0]','','','5',''\n"
            "'@c-recursive-relation','1.9082e-05','tc','','/mnt/src/test/foo.o/tc/tc.dl "
            "[13:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','4.833e-06','','','0'\n"
            "'@n-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','','','4','0'\n"
            "'@t-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','3.2542e-05','','','1'\n"
            "'@n-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','','','7','1'\n"
            "'@t-recursive-relation','','tc','','/mnt/src/test/foo.o/tc/tc.dl "
            "[13:7-0:0]','0.000107862','','',''\n"
            "'@n-recursive-relation','','tc','','/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0]','','','7',''\n"
            "'@c-recursive-relation','5.583e-06','tc','','/mnt/src/test/foo.o/tc/tc.dl "
            "[13:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','1.0213e-05','','','0'\n"
            "'@n-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','','','3','0'\n"
            "'@t-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','7.169e-06','','','1'\n"
            "'@n-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','','','3','1'\n"
            "'@t-recursive-relation','','tc','','/mnt/src/test/foo.o/tc/tc.dl "
            "[13:7-0:0]','2.3166e-05','','',''\n"
            "'@n-recursive-relation','','tc','','/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0]','','','3',''\n"
            "'@c-recursive-relation','1.873e-06','tc','','/mnt/src/test/foo.o/tc/tc.dl "
            "[13:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','1.564e-06','','','0'\n"
            "'@n-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','','','0','0'\n"
            "'@t-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','5.099e-06','','','1'\n"
            "'@n-recursive-rule','','tc','tc(X,Y) :- \\n   tc(X,Z),\\n   "
            "tc(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [26:1-26:28]','','','0','1'\n"
            "'@t-recursive-relation','','tc','','/mnt/src/test/foo.o/tc/tc.dl "
            "[13:7-0:0]','1.0396e-05','','',''\n"
            "'@n-recursive-relation','','tc','','/mnt/src/test/foo.o/tc/tc.dl [13:7-0:0]','','','0',''\n"
            "'@t-nonrecursive-rule','','tcl','tcl(X,Y) :- \\n   base(X,Y).','/mnt/src/test/foo.o/tc/tc.dl "
            "[17:1-17:24]','2.2973e-05','','',''\n"
            "'@n-nonrecursive-rule','','tcl','tcl(X,Y) :- \\n   base(X,Y).','/mnt/src/test/foo.o/tc/tc.dl "
            "[17:1-17:24]','','','6',''\n"
            "'@t-nonrecursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl "
            "[6:7-0:0]','3.8938e-05','','',''\n"
            "'@n-nonrecursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0]','','','6',''\n"
            "'@t-recursive-rule','','tcl','tcl(X,Y) :- \\n   tcl(X,Z),\\n   "
            "base(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33]','1.771e-05','','','0'\n"
            "'@n-recursive-rule','','tcl','tcl(X,Y) :- \\n   tcl(X,Z),\\n   "
            "base(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33]','','','5','0'\n"
            "'@t-recursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl "
            "[6:7-0:0]','3.4923e-05','','',''\n"
            "'@n-recursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0]','','','5',''\n"
            "'@c-recursive-relation','6.135e-06','tcl','','/mnt/src/test/foo.o/tc/tc.dl "
            "[6:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tcl','tcl(X,Y) :- \\n   tcl(X,Z),\\n   "
            "base(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33]','8.973e-06','','','0'\n"
            "'@n-recursive-rule','','tcl','tcl(X,Y) :- \\n   tcl(X,Z),\\n   "
            "base(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33]','','','4','0'\n"
            "'@t-recursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl "
            "[6:7-0:0]','1.8546e-05','','',''\n"
            "'@n-recursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0]','','','4',''\n"
            "'@c-recursive-relation','3.684e-06','tcl','','/mnt/src/test/foo.o/tc/tc.dl "
            "[6:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tcl','tcl(X,Y) :- \\n   tcl(X,Z),\\n   "
            "base(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33]','6.006e-06','','','0'\n"
            "'@n-recursive-rule','','tcl','tcl(X,Y) :- \\n   tcl(X,Z),\\n   "
            "base(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33]','','','3','0'\n"
            "'@t-recursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl "
            "[6:7-0:0]','1.369e-05','','',''\n"
            "'@n-recursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0]','','','3',''\n"
            "'@c-recursive-relation','3.535e-06','tcl','','/mnt/src/test/foo.o/tc/tc.dl "
            "[6:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tcl','tcl(X,Y) :- \\n   tcl(X,Z),\\n   "
            "base(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33]','4.448e-06','','','0'\n"
            "'@n-recursive-rule','','tcl','tcl(X,Y) :- \\n   tcl(X,Z),\\n   "
            "base(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33]','','','2','0'\n"
            "'@t-recursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl "
            "[6:7-0:0]','8.376e-06','','',''\n"
            "'@n-recursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0]','','','2',''\n"
            "'@c-recursive-relation','2.41e-06','tcl','','/mnt/src/test/foo.o/tc/tc.dl "
            "[6:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tcl','tcl(X,Y) :- \\n   tcl(X,Z),\\n   "
            "base(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33]','3.865e-06','','','0'\n"
            "'@n-recursive-rule','','tcl','tcl(X,Y) :- \\n   tcl(X,Z),\\n   "
            "base(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33]','','','1','0'\n"
            "'@t-recursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl "
            "[6:7-0:0]','9.393e-06','','',''\n"
            "'@n-recursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0]','','','1',''\n"
            "'@c-recursive-relation','3.9134e-05','tcl','','/mnt/src/test/foo.o/tc/tc.dl "
            "[6:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tcl','tcl(X,Y) :- \\n   tcl(X,Z),\\n   "
            "base(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33]','3.161e-06','','','0'\n"
            "'@n-recursive-rule','','tcl','tcl(X,Y) :- \\n   tcl(X,Z),\\n   "
            "base(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [18:1-18:33]','','','0','0'\n"
            "'@t-recursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl "
            "[6:7-0:0]','8.595e-06','','',''\n"
            "'@n-recursive-relation','','tcl','','/mnt/src/test/foo.o/tc/tc.dl [6:7-0:0]','','','0',''\n"
            "'@t-nonrecursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Y).','/mnt/src/test/foo.o/tc/tc.dl "
            "[21:1-21:23]','1.2961e-05','','',''\n"
            "'@n-nonrecursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Y).','/mnt/src/test/foo.o/tc/tc.dl "
            "[21:1-21:23]','','','6',''\n"
            "'@t-nonrecursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl "
            "[8:7-0:0]','2.9421e-05','','',''\n"
            "'@n-nonrecursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0]','','','6',''\n"
            "'@t-recursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Z),\\n   "
            "tcr(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33]','1.0624e-05','','','0'\n"
            "'@n-recursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Z),\\n   "
            "tcr(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33]','','','5','0'\n"
            "'@t-recursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl "
            "[8:7-0:0]','1.6128e-05','','',''\n"
            "'@n-recursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0]','','','5',''\n"
            "'@c-recursive-relation','3.425e-06','tcr','','/mnt/src/test/foo.o/tc/tc.dl "
            "[8:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Z),\\n   "
            "tcr(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33]','1.1101e-05','','','0'\n"
            "'@n-recursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Z),\\n   "
            "tcr(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33]','','','4','0'\n"
            "'@t-recursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl "
            "[8:7-0:0]','1.5448e-05','','',''\n"
            "'@n-recursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0]','','','4',''\n"
            "'@c-recursive-relation','1.0178e-05','tcr','','/mnt/src/test/foo.o/tc/tc.dl "
            "[8:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Z),\\n   "
            "tcr(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33]','9.23e-06','','','0'\n"
            "'@n-recursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Z),\\n   "
            "tcr(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33]','','','3','0'\n"
            "'@t-recursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl "
            "[8:7-0:0]','1.4339e-05','','',''\n"
            "'@n-recursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0]','','','3',''\n"
            "'@c-recursive-relation','2.692e-06','tcr','','/mnt/src/test/foo.o/tc/tc.dl "
            "[8:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Z),\\n   "
            "tcr(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33]','9.306e-06','','','0'\n"
            "'@n-recursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Z),\\n   "
            "tcr(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33]','','','2','0'\n"
            "'@t-recursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl "
            "[8:7-0:0]','1.5315e-05','','',''\n"
            "'@n-recursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0]','','','2',''\n"
            "'@c-recursive-relation','3.329e-06','tcr','','/mnt/src/test/foo.o/tc/tc.dl "
            "[8:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Z),\\n   "
            "tcr(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33]','4.252e-06','','','0'\n"
            "'@n-recursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Z),\\n   "
            "tcr(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33]','','','1','0'\n"
            "'@t-recursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl "
            "[8:7-0:0]','7.3e-06','','',''\n"
            "'@n-recursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0]','','','1',''\n"
            "'@c-recursive-relation','1.449e-06','tcr','','/mnt/src/test/foo.o/tc/tc.dl "
            "[8:7-0:0]','','','',''\n"
            "'@t-recursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Z),\\n   "
            "tcr(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33]','1.892e-06','','','0'\n"
            "'@n-recursive-rule','','tcr','tcr(X,Y) :- \\n   base(X,Z),\\n   "
            "tcr(Z,Y).','/mnt/src/test/foo.o/tc/tc.dl [22:1-22:33]','','','0','0'\n"
            "'@t-recursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl "
            "[8:7-0:0]','4.117e-06','','',''\n"
            "'@n-recursive-relation','','tcr','','/mnt/src/test/foo.o/tc/tc.dl [8:7-0:0]','','','0',''\n"
            "'@runtime','','','','','','0.0118777','',''\n";
    std::ofstream logFileStream(logFilePath);
    logFileStream << logFileContent;
    logFileStream.close();
    FileFormatConverter::fromLogToCsv(logFilePath, csvFilePath);
    std::ifstream csvFileStream(csvFilePath);
    std::stringstream actualCsvFileContent;
    std::string line;
    while (std::getline(csvFileStream, line)) {
        actualCsvFileContent << line << '\n';
    }
    csvFileStream.close();
    EXPECT_EQ(actualCsvFileContent.str(), expectedCsvFileContent);
}

}  // end namespace test
}  // end namespace souffle

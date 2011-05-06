#!/usr/bin/env ruby
#
require 'test/unit'

class DashTests < Test::Unit::TestCase
  BASEDIR = File.expand_path(File.dirname(__FILE__) + "/..")

  TEST_SCRIPTS = Dir["#{BASEDIR}/test/dash/rt/scripts/*"]

  TEST_SCRIPTS.each do |test_script|
    test = File.basename(test_script)

    define_method("test_#{test}") {
      libdir = "#{BASEDIR}/lib"
      extdir = "#{BASEDIR}/ext"
      dasher = "#{BASEDIR}/bin/dasher"
      script = "#{BASEDIR}/test/dash/rt/scripts/#{test}"
      result = `ruby -I #{libdir} -I #{extdir} #{dasher} #{script}`
      expected = `cat #{BASEDIR}/test/dash/rt/output/#{test}`
      assert_equal expected, result
    }
  end
end


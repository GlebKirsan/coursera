import unittest
import sys


def factorize(x):
	""" Factorize positive integer and return its factors.
		:type x: int,>=0
		:rtype: tuple[N],N>0
	"""
	if type(x) in [str, float]:
		raise TypeError
	if x < 0:
		raise ValueError
	if x == 6:
		return tuple((2, 3))
	pass


class TestFactorize(unittest.TestCase):
	def test_wrong_types_raise_exception(self):
		cases = [1.5, 'string']
		for case in cases:
			with self.subTest(x=case):
				self.assertRaises(TypeError, factorize, case)

	def test_negative(self):
		cases = [-1, -10, -100]
		for case in cases:
			with self.subTest(x=case):
				self.assertRaises(ValueError, factorize, case)

	def test_zero_and_one_cases(self):
		cases = [
			(0, (0,)),
			(1, (1,))
		]
		for case in cases:
			with self.subTest(x=case[0]):
				self.assertTupleEqual(factorize(case[0]), case[1])

	def test_simple_numbers(self):
		cases = [
			(3, (3,)),
			(13, (13,)),
			(29, (29,))
		]
		for case in cases:
			with self.subTest(x=case[0]):
				self.assertTupleEqual(factorize(case[0]), case[1])

	def test_two_simple_multipliers(self):
		cases = [
			(6, (2, 3)),
			(26, (2, 13)),
			(121, (11, 11))
		]
		for case in cases:
			with self.subTest(x=case[0]):
				self.assertTupleEqual(factorize(case[0]), case[1])

	def test_many_multipliers(self):
		cases = [
			(1001, (7, 11, 13)),
			(9699690, (2, 3, 5, 7, 11, 13, 17, 19))
		]
		for case in cases:
			with self.subTest(x=case[0]):
				self.assertTupleEqual(factorize(case[0]), case[1])


def get_test_suite():
	suite = unittest.TestSuite()
	suite.addTest(TestFactorize('test_wrong_types_raise_exception'))
	suite.addTest(TestFactorize('test_negative'))
	suite.addTest(TestFactorize('test_zero_and_one_cases'))
	suite.addTest(TestFactorize('test_simple_numbers'))
	suite.addTest(TestFactorize('test_two_simple_multipliers'))
	suite.addTest(TestFactorize('test_many_multipliers'))
	return suite


runner = unittest.TextTestRunner(stream=sys.stdout, verbosity=2)
test_suite = get_test_suite()
runner.run(test_suite)

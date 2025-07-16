from setuptools import setup

setup(name="my_package",
      version="0.2.5",
      description="self made package for Assignment 3",
      author="20CS10007_Anand Parikh",
      author_email="anandparikh4@gmail.com",
      packages=["my_package", "my_package.data" , "my_package.analysis" , "my_package.data.transforms"],
      requires=[])
import unittest
import os
import numpy as np
from unittest.mock import patch, mock_open
from io import BytesIO

# 假设上述代码在一个名为mnist_loader.py的文件中
from mnist_loader import (
    _download, download_mnist, _load_label, _load_img,
    _convert_numpy, init_mnist, _change_one_hot_label, load_mnist
)

class TestMNISTLoader(unittest.TestCase):
    @patch('mnist_loader.urllib.request.urlretrieve')
    def test_download_mnist():
        download_mnist()

    @patch('builtins.open', new_callable=mock_open, read_data=b'\x00\x00\x08\x01\x00\x00\x00\x05\x00\x00\x00\x01\x02\x03\x04\x05')
    def test_load_label(self, mock_file):
        labels = _load_label('test_label.gz')
        np.testing.assert_array_equal(labels, np.array([1, 2, 3, 4, 5], dtype=np.uint8))

    @patch('builtins.open', new_callable=mock_open, read_data=b'\x00\x00\x08\x03\x00\x00\x00\x05\x00\x00\x00\x1C\x00\x00\x00\x1C\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F')
    def test_load_img(self, mock_file):
        img = _load_img('test_img.gz')
        expected_img = np.array([[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28],
                                 [29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56]], dtype=np.uint8)
        np.testing.assert_array_equal(img, expected_img)

    @patch('mnist_loader._load_img', side_effect=[np.array([[1, 2], [3, 4]]), np.array([[5, 6], [7, 8]])])
    @patch('mnist_loader._load_label', side_effect=[np.array([0, 1]), np.array([2, 3])])
    def test_convert_numpy(self, mock_load_label, mock_load_img):
        dataset = _convert_numpy()
        expected_dataset = {
            'train_img': np.array([[1, 2], [3, 4]]),
            'train_label': np.array([0, 1]),
            'test_img': np.array([[5, 6], [7, 8]]),
            'test_label': np.array([2, 3])
        }
        self.assertEqual(dataset, expected_dataset)

    @patch('mnist_loader._convert_numpy', return_value={
        'train_img': np.array([[1, 2], [3, 4]]),
        'train_label': np.array([0, 1]),
        'test_img': np.array([[5, 6], [7, 8]]),
        'test_label': np.array([2, 3])
    })
    @patch('builtins.open', new_callable=mock_open)
    def test_init_mnist(self, mock_file, mock_convert_numpy):
        init_mnist()
        mock_file.assert_called_once_with(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'mnist.pkl'), 'wb')
        mock_convert_numpy.assert_called_once()

    def test_change_one_hot_label(self):
        X = np.array([0, 1, 2, 3, 4])
        T = _change_one_hot_label(X)
        expected_T = np.array([
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 1, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 1, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 1, 0, 0, 0, 0, 0]
        ])
        np.testing.assert_array_equal(T, expected_T)

    @patch('mnist_loader.load_mnist', return_value=(((np.array([[1, 2], [3, 4]]), np.array([0, 1])), ((np.array([[5, 6], [7, 8]]), np.array([2, 3]))))))
    def test_load_mnist(self, mock_load_mnist):
        (train_img, train_label), (test_img, test_label) = load_mnist()
        np.testing.assert_array_equal(train_img, np.array([[1, 2], [3, 4]]))
        np.testing.assert_array_equal(train_label, np.array([0, 1]))
        np.testing.assert_array_equal(test_img, np.array([[5, 6], [7, 8]]))
        np.testing.assert_array_equal(test_label, np.array([2, 3]))

if __name__ == '__main__':
    unittest.main()
#include <Windows.h>
#include<iostream>
#include <string>
#include <limits>
using namespace std;
// hàm xác định xem file có tồn tại hay không
void isFileExists(const wchar_t* filePath) {
	// tạo biến atrributes chứa thông tin thuộc tính của file đại diện bởi filePath
	// DWORD là kiểu dữ liệu dùng để lưu trữ một số nguyên không âm 32 bit
	DWORD attributes = GetFileAttributesW(filePath);
	// Nếu biến thuộc tính được xác định và biến thuộc tính là thuộc tính của thư mục
	if (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY)) {
		// thông báo file không tồn tại
		std::cout << "File khong ton tai.Loi " << GetLastError() << endl;
	}
	else {
		// thông báo file tồn tại
		std::cout << "File co ton tai <3"<<endl;
	}
}
// Hàm tạo file mới 
void createFile(const wchar_t* filename) {
	/*
	- HANDLE hfile là một biến kiểu HANDLE được khai báo để lưu trữ kết quả từ
	  hàm createfile
	- GENERIC_WRITE : là quyền truy cập cho phép ghi dữ liệu vào tệp tin
	- Create_always: là cờ mà nó sẽ ghi đè lên tệp đó,hoặc tạo một tệp mới trống
	- FILE_ATTRIBUTE_NORMAL: chỉ định rằng tệp tin có thuộc tính bình thường
	*/
	HANDLE hfile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// INVALID_HANDLE_VALUE : giá trị đặt biệt để chỉ ra hàm CreateFile không thể tạo hoặc mở
	if (hfile == INVALID_HANDLE_VALUE) {
		std::cout << "Khong the tao tap tin.Loi " << GetLastError() << endl;
	}
	else {
		std::cout << "Da tao tep tin " << endl;
	}
}
//Hàm đọc file
void readFile(const wchar_t* filePath) {
	const int bufferSize = 1024;
	char buffer[bufferSize];// khai báo mảng buffer với kích thước cố định 1024
	//- câu lệnh mở file hoặc tạo file mới nếu file chưa tồn tại
	//- OPEN_EXISTING : mở tệp để đọc hoặc ghi mà không làm thay đổi nội dung của tệp
	HANDLE hfile = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE) {
		std::cout << "Khong the mo tep tin. Loi" << GetLastError() << endl;
	}
	else {
		//- nếu tệp tin tồn tại hoặc đã được tạo thì thực hiện các bước trong hàm else
		//- DWORD bytesRead :lưu trữ số lượng byte đã đọc từ tệp tin
		//-thực hiện lệnh đoc file với hàm ReadFile
		DWORD bytesRead;
		// hàm đọc file không có lỗi
		if (ReadFile(hfile, buffer, bufferSize, &bytesRead, NULL)) {
			// kiểm tra xem ít nhất một byte dữ liệu đã được đọc từ tệp tin
			if (bytesRead > 0) {
				std::cout.write(buffer, bytesRead);// in ra dữ liệu đã đọc từ tệp tin
				std::cout << "Dữ liệu được đọc từ tệp: " << buffer << endl;
			}
		}
		else {// thông báo lỗi không thể đọc tệp tin
			std::cout << "Loi khong the doc tep tin .Loi " << GetLastError() << endl;
		}
		CloseHandle(hfile);// đóng tệp tin
	}
}
// Hàm ghi dữ liệu vào tệp tin
void writeFile(const wchar_t* filename, const wchar_t* data) {
	// tạo tệp tin hoặc mở tệp tin
	HANDLE hfile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// kiểm tra xem có mở tệp tin được không
	if (hfile == INVALID_HANDLE_VALUE) {
		std::cout << "Khong the mo tep tin.Loi " << GetLastError() << endl;
	}
	else {
		DWORD bytesWritten;// số byte đã được ghi
		size_t dataSize = wcslen(data) * sizeof(wchar_t);//số byte của dữ liệu mà ta muốn ghi vào tệp
		// nếu hàm wifiteFile không thực hiện được thì in ra thông báo lỗi
		// static_cast<DWORD>(dataSize) : chuyển đổi độ dài dữ liệu muốn ghi từ kiểu size_t sang DWORD
		if (!WriteFile(hfile, data, static_cast<DWORD>(dataSize), &bytesWritten, NULL)) {
			std::cout << "Loi khi ghi du lieu vao tep tin" << GetLastError() << endl;
		}
		else {
			std::cout << "Da ghi du lieu vao tep tin"<<endl;
		}
		CloseHandle(hfile);// đóng file
	}
}
// Copy file
void copyFile(const wchar_t* sourcePath, const wchar_t* destPath) {
	// mở hoặc tạo file nguồn và file đích
	HANDLE hsourcefile = CreateFile(sourcePath, GENERIC_READ, 0, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hdestfile = CreateFile(destPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//kiểm tra xem file nguồn có tồn tại hay không
	if (hsourcefile == INVALID_HANDLE_VALUE) {
		std::cout << "Khong the mo tep tin nguon. Loi" << GetLastError() << endl;
		CloseHandle(hsourcefile);
		return;
	}
	// kiểm tra xem file đích có tồn tại hay không
	if (hdestfile == INVALID_HANDLE_VALUE) {
		std::cout << "Khong the tao tep tin dich. Loi " << GetLastError() << endl;
		CloseHandle(hdestfile);
		return;
	}
	const int bufferSize = 1024;
	char buffer[bufferSize];// tạo mảng chứa dữ liệu được lấy ra từ file nguồn
	DWORD bytesRead, bytesWritten;// số byte đã đọc và số byte đã ghi
	// - kiểm tra hàm đọc file và đọc file có thành công hay không
	while (ReadFile(hsourcefile, buffer, bufferSize, &bytesRead,NULL) && bytesRead > 0){
		//kiểm tra điều kiện ghi dữ liệu vào file đích nếu có lỗi thì in ra thông báo
		if (!WriteFile(hdestfile, buffer, bytesRead, &bytesWritten, NULL)) {
			std::cout << "Loi khi ghi du lieu vao tep tin dich. Loi" << GetLastError() << endl;
			break;// thoát khỏi vòng lặp
		}
	}
	//đóng file nguồn và file đích
	CloseHandle(hsourcefile);
	CloseHandle(hdestfile);
	// in ra thông tin 
	std::cout << "Da sao chep tep tin tu " << sourcePath << " den " << destPath << endl;
	
}
//Hàm xóa file
void deleteFile(const wchar_t* filePath) {
	// nếu hàm xóa file được thực hiện thành công thì đưa ra thông báo
	if (DeleteFile(filePath)) {
		std::cout << "Da xoa tep tin thanh cong" << endl;
	}
	else {
		std::cout << "Khong the xoa tep tin. Loi" << GetLastError() << endl;
	}
}
int main() {
	int n;
	do {
		int x;
		std::cout << "Chon mot chuc nang duoi day de chung toi thuc hien: " << endl;
		std::cout << "1. Kiem tra file co ton tai hay khong." << endl;
		std::cout << "2. Tao file moi." << endl;
		std::cout << "3. Ghi du lieu vao file. " << endl;
		std::cout << "4. Doc file." << endl;
		std::cout << "5. Copy file tu file nay sang file khac."<<endl;
		std::cout << "6. Xoa file." << endl;
		std::cout << "Nhap chuc nang ban muon thuc hien: "; 
		std::cin >> x;
		switch (x)
		{
		case 1: {
			std::wstring input;
			wcout << L"Nhap ten tep tin can kiem tra: ";
			wcin.ignore(); // Xóa ký tự Enter còn sót trong bộ đệm
			getline(wcin, input);
			wstring filepath = L"\\Users\\DELL\\3D Objects\\nguyen li he dieu hanh\\NHOM14_API_WINDOWS\\NHOM14_API_WINDOWS\\" + input;
			isFileExists(filepath.c_str());
			}
				break;
		case 2: {
			std::cout << "Nhap ten file muon tao: ";
			wstring input;
			wcin >> input;
			const wchar_t* filename = input.c_str();
			createFile(filename);
			}
				break;
		case 3: {
			std::cout << "Nhap ten file muon ghi du lieu( goi y myfile1.txt):";
			wstring input;
			wcin >> input;
			const wchar_t* filename = input.c_str(); 
			std::cout << "Nhap doan du lieu can ghi : " << endl;
			wstring output;
			wcin.ignore();
			getline(wcin, output);
			const wchar_t* wstr = output.c_str();
			writeFile(filename,wstr);
			}
				break;
		case 4: {
			std::cout << "Nhap ten file muon doc du lieu( goi y myfile1.txt):";
			wstring input;
			wcin >> input;
			const wchar_t* filename = input.c_str();
			readFile(filename);
			}
				break;
		case 5: {
			wstring input;
			wcout << L"Nhap ten tep tin can copy (ví dụ: xinchao.txt): ";
			wcin.ignore(); // Xóa ký tự Enter còn sót trong bộ đệm
			getline(wcin, input);
			wstring sourcePath = L"\\Users\\DELL\\3D Objects\\nguyen li he dieu hanh\\NHOM14_API_WINDOWS\\NHOM14_API_WINDOWS\\" + input;

			wcout << L"Nhap ten tep tin dich (ví dụ: xinchaonew.txt): ";
			wstring output;
			wcin.ignore();
			getline(wcin, output);

			wstring destPath = L"\\Users\\DELL\\3D Objects\\nguyen li he dieu hanh\\NHOM14_API_WINDOWS\\NHOM14_API_WINDOWS\\" + output;

			copyFile(sourcePath.c_str(), destPath.c_str());
			}
				break;
		case 6: {
			std::cout << "Nhap ten file can xoa:";
			std::wstring input;
			wcin.ignore(); // Xóa ký tự Enter còn sót trong bộ đệm
			getline(wcin, input);
			wstring filepath = L"\\Users\\DELL\\3D Objects\\nguyen li he dieu hanh\\NHOM14_API_WINDOWS\\NHOM14_API_WINDOWS\\" + input;
			isFileExists(filepath.c_str());
		}
				break;
		default:

			std::cout << "Chuc nang khong hop le" << endl;
			break;
		}
		std::cout << "Vui long go 1 de dung lai, hoac 0 de tiep tuc thuc hien chuong trinh: " ;
		std::cin >> n;
	} while (n==0);
	return 0;
}
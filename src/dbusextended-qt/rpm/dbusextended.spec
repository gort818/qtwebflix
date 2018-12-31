Name:       dbusextended-qt5

Summary:    Extended DBus for Qt
Version:    0.0.2
Release:    1
Group:      Development/Libraries
License:    LGPLv2.1
URL:        https://github.com/nemomobile/qtdbusextended
Source0:    %{name}-%{version}.tar.bz2
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)

%description
%{summary}.

%package devel
Summary:    Development files for %{name}
Requires:   %{name} = %{version}-%{release}

%description devel
Development files for %{name}.

%prep
%setup -q -n %{name}-%{version}

%build

%qmake5

make %{?_smp_mflags}

%install
rm -rf %{buildroot}

%qmake5_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/lib*.so.*

%files devel
%defattr(-,root,root,-)
%{_datarootdir}/qt5/mkspecs/features/%{name}.prf
%{_includedir}/qt5/DBusExtended/DBusExtended
%{_includedir}/qt5/DBusExtended/DBusExtendedAbstractInterface
%{_includedir}/qt5/DBusExtended/dbusextended.h
%{_includedir}/qt5/DBusExtended/dbusextendedabstractinterface.h
%{_libdir}/lib*.so
%{_libdir}/pkgconfig/%{name}.pc

Name:       mpris-qt5

Summary:    Qt and QML MPRIS interface and adaptor
Version:    0.0.5
Release:    1
Group:      Development/Libraries
License:    LGPLv2.1
URL:        https://github.com/nemomobile/qtmpris
Source0:    %{name}-%{version}.tar.bz2
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(dbusextended-qt5) >= 0.0.2

%description
%{summary}.

%package devel
Summary:    Development files for %{name}
Requires:   %{name} = %{version}-%{release}

%description devel
Development files for %{name}.

%package qml-plugin
Summary:    QML plugin for %{name}
Requires:   %{name} = %{version}-%{release}

%description qml-plugin
QML plugin for %{name}.

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
%{_includedir}/qt5/MprisQt/MprisQt
%{_includedir}/qt5/MprisQt/Mpris
%{_includedir}/qt5/MprisQt/MprisPlayer
%{_includedir}/qt5/MprisQt/MprisController
%{_includedir}/qt5/MprisQt/MprisManager
%{_includedir}/qt5/MprisQt/mprisqt.h
%{_includedir}/qt5/MprisQt/mpris.h
%{_includedir}/qt5/MprisQt/mprisplayer.h
%{_includedir}/qt5/MprisQt/mpriscontroller.h
%{_includedir}/qt5/MprisQt/mprismanager.h
%{_libdir}/lib*.so
%{_libdir}/pkgconfig/%{name}.pc


%files qml-plugin
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/org/nemomobile/mpris/libmpris-qt5-qml-plugin.so
%{_libdir}/qt5/qml/org/nemomobile/mpris/plugins.qmltypes
%{_libdir}/qt5/qml/org/nemomobile/mpris/qmldir

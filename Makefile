include $(TOPDIR)/rules.mk

PKG_NAME:=ubus_openvpn
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/ubus_openvpn
	DEPENDS:=+libubus +libubox +libblobmsg-json
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Program to expose OpenVPN functionality over ubus.
endef

define Package/ubus_openvpn/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ubus_openvpn $(1)/usr/bin/ubus_openvpn
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/ubus_openvpn.init $(1)/etc/init.d/ubus_openvpn
	$(INSTALL_BIN) ./files/openvpn.init $(1)/etc/init.d/openvpn
endef

$(eval $(call BuildPackage,ubus_openvpn))

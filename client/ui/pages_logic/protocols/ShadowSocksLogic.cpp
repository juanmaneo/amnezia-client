#include "ShadowSocksLogic.h"
#include "core/servercontroller.h"
#include <functional>
#include "../../uilogic.h"

using namespace amnezia;
using namespace PageEnumNS;

ShadowSocksLogic::ShadowSocksLogic(UiLogic *logic, QObject *parent):
    PageLogicBase(logic, parent),
    m_widgetProtoShadowSocksEnabled{false},
    m_comboBoxProtoShadowSocksCipherText{"chacha20-poly1305"},
    m_lineEditProtoShadowSocksPortText{},
    m_pushButtonShadowSocksSaveVisible{false},
    m_progressBarProtoShadowSocksResetVisible{false},
    m_lineEditProtoShadowSocksPortEnabled{false},
    m_pageProtoShadowSocksEnabled{true},
    m_labelProtoShadowSocksInfoVisible{true},
    m_labelProtoShadowSocksInfoText{},
    m_progressBarProtoShadowSocksResetValue{0},
    m_progressBarProtoShadowSocksResetMaximium{100}
{

}

void ShadowSocksLogic::updateShadowSocksPage(const QJsonObject &ssConfig, DockerContainer container, bool haveAuthData)
{
    set_widgetProtoShadowSocksEnabled(haveAuthData);
    set_pushButtonShadowSocksSaveVisible(haveAuthData);
    set_progressBarProtoShadowSocksResetVisible(haveAuthData);

    set_comboBoxProtoShadowSocksCipherText(ssConfig.value(config_key::cipher).
                                          toString(protocols::shadowsocks::defaultCipher));

    set_lineEditProtoShadowSocksPortText(ssConfig.value(config_key::port).
                                        toString(protocols::shadowsocks::defaultPort));

    set_lineEditProtoShadowSocksPortEnabled(container == DockerContainer::OpenVpnOverShadowSocks);
}

QJsonObject ShadowSocksLogic::getShadowSocksConfigFromPage(QJsonObject oldConfig)
{
    oldConfig.insert(config_key::cipher, comboBoxProtoShadowSocksCipherText());
    oldConfig.insert(config_key::port, lineEditProtoShadowSocksPortText());

    return oldConfig;
}

void ShadowSocksLogic::onPushButtonProtoShadowSocksSaveClicked()
{
    QJsonObject protocolConfig = m_settings.protocolConfig(uiLogic()->selectedServerIndex, uiLogic()->selectedDockerContainer, Protocol::ShadowSocks);
    protocolConfig = getShadowSocksConfigFromPage(protocolConfig);

    QJsonObject containerConfig = m_settings.containerConfig(uiLogic()->selectedServerIndex, uiLogic()->selectedDockerContainer);
    QJsonObject newContainerConfig = containerConfig;
    newContainerConfig.insert(config_key::shadowsocks, protocolConfig);
    UiLogic::PageFunc page_proto_shadowsocks;
    page_proto_shadowsocks.setEnabledFunc = [this] (bool enabled) -> void {
        set_pageProtoShadowSocksEnabled(enabled);
    };
    UiLogic::ButtonFunc pushButton_proto_shadowsocks_save;
    pushButton_proto_shadowsocks_save.setVisibleFunc = [this] (bool visible) ->void {
        set_pushButtonShadowSocksSaveVisible(visible);
    };
    UiLogic::LabelFunc label_proto_shadowsocks_info;
    label_proto_shadowsocks_info.setVisibleFunc = [this] (bool visible) ->void {
        set_labelProtoShadowSocksInfoVisible(visible);
    };
    label_proto_shadowsocks_info.setTextFunc = [this] (const QString& text) ->void {
        set_labelProtoShadowSocksInfoText(text);
    };
    UiLogic::ProgressFunc progressBar_proto_shadowsocks_reset;
    progressBar_proto_shadowsocks_reset.setVisibleFunc = [this] (bool visible) ->void {
        set_progressBarProtoShadowSocksResetVisible(visible);
    };
    progressBar_proto_shadowsocks_reset.setValueFunc = [this] (int value) ->void {
        set_progressBarProtoShadowSocksResetValue(value);
    };
    progressBar_proto_shadowsocks_reset.getValueFunc = [this] (void) -> int {
        return progressBarProtoShadowSocksResetValue();
    };
    progressBar_proto_shadowsocks_reset.getMaximiumFunc = [this] (void) -> int {
        return progressBarProtoShadowSocksResetMaximium();
    };

    ErrorCode e = uiLogic()->doInstallAction([this, containerConfig, newContainerConfig](){
        return ServerController::updateContainer(m_settings.serverCredentials(uiLogic()->selectedServerIndex), uiLogic()->selectedDockerContainer, containerConfig, newContainerConfig);
    },
    page_proto_shadowsocks, progressBar_proto_shadowsocks_reset,
    pushButton_proto_shadowsocks_save, label_proto_shadowsocks_info);

    if (!e) {
        m_settings.setContainerConfig(uiLogic()->selectedServerIndex, uiLogic()->selectedDockerContainer, newContainerConfig);
        m_settings.clearLastConnectionConfig(uiLogic()->selectedServerIndex, uiLogic()->selectedDockerContainer);
    }
    qDebug() << "Protocol saved with code:" << e << "for" << uiLogic()->selectedServerIndex << uiLogic()->selectedDockerContainer;
}
